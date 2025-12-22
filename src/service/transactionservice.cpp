#include "transactionservice.h"
#include "../common/converters.h"
#include <cmath>

namespace Services {

/*
void Endpoints::TransactionBefore::privateInvoke(MessageContext &ctx)
{
    const QJsonObject request = ctx.jsonRequest;
    const QVariant toVar      = request.value(toStr(JsonField::ToAcc)).toVariant();
    const QString amountStr   = request.value(toStr(JsonField::Amount)).toString();
    const QVariant fromVar    = request.value(toStr(JsonField::FromAcc)).toVariant();

    bool isCorrect = false;
    qint64 toId       = toVar.toLongLong();
    QString toIban    = toVar.toString().trimmed();
    std::optional<Models::Account> toAccOpt = std::nullopt;

    // Only attempt ID lookup when a positive id is provided
    if (toId > 0) {
        toAccOpt = accRepo->getById(toId);
    }
    // Only attempt IBAN lookup if non-empty IBAN string provided
    if (!toAccOpt.has_value() && !toIban.isEmpty()) {
        toAccOpt = accRepo->getByIban(toIban);
    }
    if (!toAccOpt.has_value())
    {
        responce(ctx.jsonResponce, BeforeTransferInfo{std::nullopt, true, "Incorrect to account"});
        return;
    }
    ///CHECK
    std::optional<Models::Account> fromAccOpt = std::nullopt;
    if (!fromVar.isNull()) {
        qint64 fromId = fromVar.toLongLong();
        QString fromIban = fromVar.toString().trimmed();
        if (fromId > 0) {
            fromAccOpt = accRepo->getById(fromId);
        }
        if (!fromAccOpt.has_value() && !fromIban.isEmpty()) {
            fromAccOpt = accRepo->getByIban(fromIban);
        }
    }

    // Disallow operations with deleted/frozen/system accounts
    auto isBadStatus = [](const Models::Account &a){
        const QString st = a.status.value_or("active");
        return (st == "frozen" || st == "deleted" || st == "system" || st == "blocked" || st == "closed");
    };
    if (toAccOpt.has_value() && isBadStatus(toAccOpt.value())) {
        responce(ctx.jsonResponce, BeforeTransferInfo{std::nullopt, true, "Recipient account is not available"});
        return;
    }
    if (fromAccOpt.has_value() && isBadStatus(fromAccOpt.value())) {
        responce(ctx.jsonResponce, BeforeTransferInfo{std::nullopt, true, "Sender account is not available"});
        return;
    }

    QString rateStr = "1";
    qint64 resultCents = 0;
    if (fromAccOpt.has_value() && exchange) {
        Enums::Currency fromCur = Enums::fromStr(fromAccOpt->currency, Enums::Currency::BYN);
        Enums::Currency toCur = Enums::fromStr(toAccOpt->currency, Enums::Currency::BYN);
        ExchangeData ex = exchange->get(fromCur, toCur);
        rateStr = ex.exchangeRateStr;
    }

    QString comStr = "";
    if (com) comStr = com->commissionPercentString();

    if (!amountStr.isEmpty()) {
        qint64 amountCents = Money::toCents(amountStr);
        qint64 commCents = com ? com->computeCommissionCents(amountCents) : 0;
        qint64 afterCommission = amountCents - commCents;
        if (afterCommission < 0) afterCommission = 0;

        // if we know sender account, ensure sender has enough balance (before conversion)
        if (fromAccOpt.has_value()) {
            if (!fromAccOpt->balance.has_value() || !Money::check(fromAccOpt->balance.value(), true)) {
                responce(ctx.jsonResponce, BeforeTransferInfo{std::nullopt, true, "Incorrect sender balance"});
                return;
            }
            qint64 fromBal = Money::toCents(fromAccOpt->balance.value());
            if (amountCents > fromBal) {
                responce(ctx.jsonResponce, BeforeTransferInfo{std::nullopt, true, "Insufficient funds"});
                return;
            }
        }

        // compute final credited amount in recipient currency
        qint64 creditCents = afterCommission;
        if (fromAccOpt.has_value() && exchange && fromAccOpt->currency != toAccOpt->currency) {
            Enums::Currency fromCur = Enums::fromStr(fromAccOpt->currency, Enums::Currency::BYN);
            Enums::Currency toCur = Enums::fromStr(toAccOpt->currency, Enums::Currency::BYN);
            ExchangeData ex = exchange->get(fromCur, toCur);
            double rate = ex.exchangeRate;
            creditCents = static_cast<qint64>(std::llround(afterCommission * rate));
        }
        resultCents = creditCents;
    }

    QString resultAmountStr = Money::fromCents(resultCents);
    BeforeTransferInfo::Res res {comStr, rateStr, resultAmountStr, toAccOpt.value()};

    responce(ctx.jsonResponce, BeforeTransferInfo{res, false, ""});
    ///
}
*/
/*
struct BeforeTransferInfo
{
    QString comission = "";
    QString exchangeRate = "";
    QString resultAmount = "";
    std::optional<Models::Account> to_acc = std::null_opt;
    bool isAllowed;
    QString error;
};
*/
BeforeTransferInfo TransactionService::getBeforeTransferInfo(const QString &toVar, const QString &amount, const QString &fromVar)
{
    QString errorOut;

    auto accsAccessibility = checkFromToAccessibility(fromVar, toVar, errorOut);
    if (!std::get<2>(accsAccessibility))
        return {
            .isAllowed = false,
            .error = errorOut
        }; //прикольно))
        // return { "", "", "", std::nullopt, false, errorOut};

    Models::Account fromAcc = std::get<0>(accsAccessibility);
    Models::Account toAcc   = std::get<1>(accsAccessibility);

    auto moneyAccessibility = checkMoneyAccessibility(fromAcc.balance.value_or(""), toAcc.balance.value_or(""), amount, errorOut);
    if (!moneyAccessibility.second)
        return {
            .to_acc = toAcc,
            .isAllowed = false,
            .error = errorOut
        };
    qint64 fromBal  = moneyAccessibility.first.fromBal;
    qint64 toBal    = moneyAccessibility.first.toBal;
    qint64 transfer = moneyAccessibility.first.transfer;


    qint64 commissionCents = m_com->computeCommissionCents(transfer);
    qint64 transferAfterCommission = transfer - commissionCents;
    if (transferAfterCommission < 0)
        transferAfterCommission = 0;

    auto exchangeRateAccessibily = checkExchangeRateAccessibility(fromAcc.currency, toAcc.currency, errorOut);
    if (!exchangeRateAccessibily.second)
        return {
            .comission = m_com->commissionPercentString() + " : " + Money::fromCents(commissionCents),
            .to_acc = toAcc,
            .isAllowed = false,
            .error = errorOut
        };
    double rate = exchangeRateAccessibily.first;
    qint64 creditCents = static_cast<qint64>(std::llround(transferAfterCommission * rate));

    // qint64 newFrom = fromBal - transfer;
    // qint64 newTo = toBal + creditCents;
    return {
        .comission = m_com->commissionPercentString() + " : " + Money::fromCents(commissionCents),
        .exchangeRate = QString::number(rate),
        .resultAmount = QString::number(creditCents),
        .to_acc = toAcc,
        .isAllowed = true,
        .error = errorOut
    };
}

std::optional<Models::Transaction> TransactionService::createTransfer(const QVariant &fromVar,
                                                                     const QVariant &toVar,
                                                                     const QString &amount,
                                                                     const QString &description,
                                                                     QString &errorOut)
{

    auto accsAccessibility = checkFromToAccessibility(fromVar, toVar, errorOut);
    if (!std::get<2>(accsAccessibility))
        return std::nullopt;
    Models::Account fromAcc = std::get<0>(accsAccessibility);
    Models::Account toAcc   = std::get<1>(accsAccessibility);

    auto moneyAccessibility = checkMoneyAccessibility(fromAcc.balance.value_or(""), toAcc.balance.value_or(""), amount, errorOut);
    if (!moneyAccessibility.second)
        return std::nullopt;
    qint64 fromBal  = moneyAccessibility.first.fromBal;
    qint64 toBal    = moneyAccessibility.first.toBal;
    qint64 transfer = moneyAccessibility.first.transfer;


    qint64 commissionCents = m_com->computeCommissionCents(transfer);
    qint64 transferAfterCommission = transfer - commissionCents;
    if (transferAfterCommission < 0)
        transferAfterCommission = 0;

    auto exchangeRateAccessibily = checkExchangeRateAccessibility(fromAcc.currency, toAcc.currency, errorOut);
    if (!exchangeRateAccessibily.second)
        return std::nullopt;
    double rate = exchangeRateAccessibily.first;
    qint64 creditCents = static_cast<qint64>(std::llround(transferAfterCommission * rate));

    qint64 newFrom = fromBal - transfer;
    qint64 newTo = toBal + creditCents;

    //до этого момента BeforeTransferInfo содержит +- идентичный код

    if (!(m_accs->updateBalance(fromAcc.id, Money::fromCents(newFrom)) &&
          m_accs->updateBalance(toAcc.id, Money::fromCents(newTo))))
    {
        errorOut = "Failed to update balances";
        return std::nullopt;
    }

    Models::Transaction tFrom;
    tFrom.account_id                = fromAcc.id;
    tFrom.counterparty_account_id   = toAcc.id;
    tFrom.amount                    = Money::fromCents(transfer);
    tFrom.currency                  = fromAcc.currency;
    tFrom.type                      = QStringLiteral("debit");
    tFrom.description               = description;
    tFrom.status                    = QStringLiteral("done");
    {
        QJsonObject md;
        md[toStr(Common::JsonField::Comission)]    = m_com->commissionPercentString();
        md[toStr(Common::JsonField::ExchangeRate)] = rate;
        tFrom.metadata = md;
    }

    Models::Transaction tTo;
    tTo.account_id                  = toAcc.id;
    tTo.counterparty_account_id     = fromAcc.id;
    tTo.amount                      = Money::fromCents(creditCents);
    tTo.currency                    = toAcc.currency;
    tTo.type                        = QStringLiteral("credit");
    tTo.description                 = description;
    tTo.status                      = QStringLiteral("done");
    {
        QJsonObject md;
        md[toStr(Common::JsonField::Comission)]    = m_com->commissionPercentString();
        md[toStr(Common::JsonField::ExchangeRate)] = rate;
        tTo.metadata = md;
    }

    const std::optional<Models::Transaction> cFrom = m_txs->addTransaction(tFrom);
    const std::optional<Models::Transaction> cTo   = m_txs->addTransaction(tTo);
    if (cFrom.has_value() && cTo.has_value())
        return cTo;
    errorOut = "Failed to create transactions";
    return std::nullopt;
}

std::tuple<Models::Account, Models::Account, bool> TransactionService::checkFromToAccessibility(const QVariant &fromVar, const QVariant &toVar, QString &errorOut)
{
    qint64 fromId    = fromVar.toLongLong();
    qint64 toId      = toVar.toLongLong();
    QString fromIban = fromVar.toString();
    QString toIban   = toVar.toString();
    Models::Account fromAcc;
    Models::Account toAcc;

    std::optional<Models::Account> fromAccOpt = m_accs->getById(fromId);
    if (!fromAccOpt.has_value()) {
        fromAccOpt = m_accs->getByIban(fromIban);
        if (!fromAccOpt.has_value())
        {
            errorOut = "Incorrect from account";
            return std::make_tuple(fromAcc, toAcc, false);
        }
    }
    std::optional<Models::Account> toAccOpt = m_accs->getById(toId);
    if (!toAccOpt.has_value()) {
        toAccOpt = m_accs->getByIban(toIban);
        if (!toAccOpt.has_value())
        {
            errorOut = "Incorrect to account";
            return std::make_tuple(fromAcc, toAcc, false);
        }
    }

    fromAcc = fromAccOpt.value();
    toAcc = toAccOpt.value();

    if (fromAcc.id == toAcc.id)
    {
        errorOut = "Same account";
        return std::make_tuple(fromAcc, toAcc, false);
    }

    auto isBadStatus = [](const Models::Account &a)
    {
        const QString st = a.status.value_or("active");
        return (st == "frozen" || st == "deleted" || st == "system" || st == "blocked" || st == "closed");
    };
    if (isBadStatus(fromAcc))
    {
        errorOut = "From account is not available";
        return std::make_tuple(fromAcc, toAcc, false);
    }
    if (isBadStatus(toAcc))
    {
        errorOut = "To account is not available";
        return std::make_tuple(fromAcc, toAcc, false);
    }
    return std::make_tuple(fromAcc, toAcc, true);
}

std::pair<qint64, bool> TransactionService::checkExchangeRateAccessibility(const QString &fromCurrency, const QString &toCurrency, QString &errorOut)
{
    double rate = 1.00;
    auto fromCurOpt = Enums::fromStr(fromCurrency, Enums::Currency::BYN);
    auto toCurOpt   = Enums::fromStr(toCurrency, Enums::Currency::BYN);
    if (!fromCurOpt.second || !toCurOpt.second)
    {
        errorOut = "Unsupported currencies";
        return std::make_pair(rate, false);
    }
    Enums::Currency fromCur = fromCurOpt.first;
    Enums::Currency toCur   = toCurOpt.first;

    if (fromCur != toCur) {
        if (!m_curEx)
        {
            errorOut = "Exchange service unavailable";
            return std::make_pair(rate, false);
        }

        //TODO: устранить 1.0
        ExchangeData ex = m_curEx->get(fromCur, toCur);
        rate = ex.exchangeRate;
        if (rate == 1.0)
        {
            errorOut = "Exchange rate unavailable";
            return std::make_pair(rate, false);
        }
    }
    return std::make_pair(rate, true);
}

std::pair<TransactionService::Cents, bool> TransactionService::checkMoneyAccessibility(const QString &fromBalStr, const QString &toBalStr, const QString &trasferStr, QString &errorOut)
{
    std::pair<qint64, bool> fromBal  = Money::toCents(fromBalStr, false);
    std::pair<qint64, bool> transfer = Money::toCents(trasferStr, false);
    std::pair<qint64, bool> toBal    = Money::toCents(toBalStr, false);
    Cents cents {fromBal.first, toBal.first, transfer.first };
    if (!fromBal.first || !fromBal.second)
    {
        errorOut = "From balance is empty or incorrect";
        return std::make_pair(cents, false);
    }

    if (!toBal.second)
    {
        errorOut = "To balance is incorrect";
        return std::make_pair(cents, false);
    }

    if (!transfer.first || !transfer.second)
    {
        errorOut = "Transfer sum is empty or incorrect";
        return std::make_pair(cents, false);
    }

    if (transfer.first <= 0 || fromBal.first < transfer.first)
    {
        errorOut = "Insufficient funds or invalid amount";
        return std::make_pair(cents, false);
    }
    return std::make_pair(cents, true);
}

} // namespace Services
#include "transactionservice.h"


