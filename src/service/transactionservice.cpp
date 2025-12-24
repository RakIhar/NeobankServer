#include "transactionservice.h"
#include "../common/converters.h"
#include <cmath>

namespace Services
{

using namespace std;

optional<TransactionService::TransferValidationResult> TransactionService::validateAndCalculate(const QVariant &fromVar, const QVariant &toVar, const QString &amount, QString &errorOut)
{
    optional<TransactionService::TransferValidationResult> res = nullopt;

    auto accs = checkFromToAccessibility(fromVar, toVar, errorOut);
    if (accs.has_value())
    {
        auto money = checkMoneyAccessibility(accs->first.balance.value_or(""), accs->second.balance.value_or(""), amount, errorOut);
        if (money.has_value())
        {
            auto rateOpt = checkExchangeRateAccessibility(accs->first.currency, accs->second.currency, errorOut);
            if (rateOpt.has_value())
            {
                qint64 comm = m_com->computeCommissionCents(money->transfer);
                qint64 afterComm = std::max(0LL, money->transfer - comm);
                qint64 credit = static_cast<qint64>(std::llround(afterComm * *rateOpt));

                res = {
                    .fromAcc = accs->first,
                    .toAcc = accs->second,
                    .transferCents = money->transfer,
                    .commissionCents = comm,
                    .creditCents = credit,
                    .rate = rateOpt.value(),
                    .fromBal = money->fromBal,
                    .toBal = money->toBal
                };
            }
        }
    }
    return res;
}

BeforeTransferInfo TransactionService::getBeforeTransferInfo(const QString &toVar, const QString &amount, const QString &fromVar)
{
    QString errorOut;
    auto res = validateAndCalculate(fromVar, toVar, amount, errorOut);

    BeforeTransferInfo info;
    info.isAllowed = res.has_value();
    info.error = errorOut;

    if (res.has_value())
    {
        info.to_acc = res->toAcc;
        info.comission = m_com->commissionPercentString() + " : " + Money::fromCents(res->commissionCents);
        info.exchangeRate = QString::number(res->rate);
        info.resultAmount = Money::fromCents(res->creditCents);
    }
    else
    {
        auto accs = checkFromToAccessibility(fromVar, toVar, errorOut);
        if (accs)
            info.to_acc = accs->second;
    }
    return info;
}

optional<Models::Transaction> TransactionService::createTransfer(const QVariant &fromVar,
                                                                 const QVariant &toVar,
                                                                 const QString &amount,
                                                                 const QString &description,
                                                                 QString &errorOut)
{
    optional<Models::Transaction> trRes = nullopt;

    //TODO: это тихий ужас, куча update без транзакций, безусловно исправить и код здесь, и код репы
    auto res = validateAndCalculate(fromVar, toVar, amount, errorOut);
    if (res.has_value())
    {
        qint64 newFrom = res->fromBal - res->transferCents;
        qint64 newTo = res->toBal + res->creditCents;

        if (!(m_accs->updateBalance(res->fromAcc.id, Money::fromCents(newFrom)) &&
              m_accs->updateBalance(res->toAcc.id, Money::fromCents(newTo))))
            errorOut = "Failed to update balances";
        else
        {
            Models::Transaction tFrom;
            tFrom.account_id                = res->fromAcc.id;
            tFrom.counterparty_account_id   = res->toAcc.id;
            tFrom.amount                    = Money::fromCents(res->transferCents);
            tFrom.currency                  = res->fromAcc.currency;
            tFrom.type                      = QStringLiteral("debit");
            tFrom.description               = description;
            tFrom.status                    = QStringLiteral("done");
            {
                QJsonObject md;
                md[toStr(Common::JsonField::Comission)]    = m_com->commissionPercentString();
                md[toStr(Common::JsonField::ExchangeRate)] = res->rate;
                tFrom.metadata = md;
            }

            Models::Transaction tTo;
            tTo.account_id                  = res->toAcc.id;
            tTo.counterparty_account_id     = res->fromAcc.id;
            tTo.amount                      = Money::fromCents(res->creditCents);
            tTo.currency                    = res->toAcc.currency;
            tTo.type                        = QStringLiteral("credit");
            tTo.description                 = description;
            tTo.status                      = QStringLiteral("done");
            {
                QJsonObject md;
                md[toStr(Common::JsonField::Comission)]    = m_com->commissionPercentString();
                md[toStr(Common::JsonField::ExchangeRate)] = res->rate;
                tTo.metadata = md;
            }

            const std::optional<Models::Transaction> cFrom = m_txs->addTransaction(tFrom);
            const std::optional<Models::Transaction> cTo   = m_txs->addTransaction(tTo);

            if (cFrom.has_value() && cTo.has_value())
                trRes = cTo;
            else
                errorOut = "Failed to create transactions";
        }
    }
    return trRes;
}

optional<pair<Models::Account, Models::Account>> TransactionService::checkFromToAccessibility(const QVariant &fromVar, const QVariant &toVar, QString &errorOut)
{
    qint64 fromId    = fromVar.toLongLong();
    qint64 toId      = toVar.toLongLong();
    QString fromIban = fromVar.toString();
    QString toIban   = toVar.toString();
    Models::Account fromAcc;
    Models::Account toAcc;
    optional<pair<Models::Account, Models::Account>> res = nullopt;

    auto getAcc = [&](const qint64 id, const QString& iban) -> std::optional<Models::Account>
    {
        std::optional<Models::Account> resAcc = m_accs->getById(id);
        if (!resAcc.has_value()) {
            resAcc = m_accs->getByIban(iban);
            if (!resAcc.has_value())
                errorOut = "Incorrect from account";
        }
        return resAcc;
    };

    std::optional<Models::Account> fromAccOpt = getAcc(fromId, fromIban);
    std::optional<Models::Account> toAccOpt   = getAcc(toId, toIban);

    if      (!fromAccOpt.has_value())
        errorOut = "Incorrect from account";
    else if (!toAccOpt.has_value())
        errorOut = "Incorrect to account";
    else
    {
        fromAcc = fromAccOpt.value();
        toAcc = toAccOpt.value();

        if (fromAcc.id == toAcc.id)
            errorOut = "Same account";
        else if (Common::fromStr(fromAcc.status.value_or("")) != Common::AccStatus::Active)
            errorOut = "From account is not available";
        else if (Common::fromStr(toAcc.status.value_or(""))   != Common::AccStatus::Active)
            errorOut = "To account is not available";
        else
            res = make_pair(fromAcc, toAcc);
    }
    return res;
}

optional<double> TransactionService::checkExchangeRateAccessibility(const QString &fromCurrency, const QString &toCurrency, QString &errorOut)
{
    optional<double> rate = nullopt;
    auto fromCurOpt = Enums::fromStr(fromCurrency, Enums::Currency::BYN);
    auto toCurOpt   = Enums::fromStr(toCurrency, Enums::Currency::BYN);
    if (fromCurOpt.second && toCurOpt.second)
    {
        Enums::Currency fromCur = fromCurOpt.first;
        Enums::Currency toCur   = toCurOpt.first;

        if (fromCur != toCur) {
            ExchangeData ex = m_curEx->get(fromCur, toCur);

            if (!ex.exchangeRate.has_value())
                errorOut = "Exchange rate unavailable";
            rate = ex.exchangeRate;
        }
    }
    else
        errorOut = "Unsupported currencies";

    return rate;
}

optional<TransactionService::Cents> TransactionService::checkMoneyAccessibility(const QString &fromBalStr, const QString &toBalStr, const QString &trasferStr, QString &errorOut)
{
    pair<qint64, bool> fromBal  = Money::toCents(fromBalStr, false);
    pair<qint64, bool> transfer = Money::toCents(trasferStr, false);
    pair<qint64, bool> toBal    = Money::toCents(toBalStr, false);
    Cents cents {fromBal.first, toBal.first, transfer.first };
    optional<Cents> res = nullopt;

    if (!fromBal.first || !fromBal.second)
        errorOut = "From balance is empty or incorrect";
    else if (!toBal.second)
        errorOut = "To balance is incorrect";
    else if (!transfer.first || !transfer.second)
        errorOut = "Transfer sum is empty or incorrect";
    else if (transfer.first <= 0 || fromBal.first < transfer.first)
        errorOut = "Insufficient funds or invalid amount";
    else
        res = cents;
    return res;
}

}
#include "transactionservice.h"


