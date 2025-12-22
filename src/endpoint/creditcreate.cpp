#include "creditcreate.h"
#include "../common/converters.h"

void Endpoints::CreditCreate::privateInvoke(MessageContext &ctx)
{
    const QJsonObject request = ctx.jsonRequest;
    const QString amount = request.value(toStr(JsonField::Amount)).toString();

    quint64 accId = request.value(toStr(JsonField::AccountId)).toVariant().toLongLong();
    bool isCorrect = false;

    std::optional<Models::Account> accOpt = accRepo->getById(accId);
    if (!accOpt.has_value())
    {
        errorResponceTemplate(ctx.jsonResponce, QStringLiteral("Incorrect from account"));
        return;
    }
    Models::Account acc = accOpt.value();

    std::function<bool(const Models::Account&)> isBadStatus = [](const Models::Account &a) -> bool {
        const QString stStr = a.status.value_or("");
        AccStatus st = fromStr(stStr, AccStatus::Deleted);
        return st != AccStatus::Active;
    };

    if (isBadStatus(acc)) {
        errorResponceTemplate(ctx.jsonResponce, QStringLiteral("Account is not available for credit"));
        return;
    }

    const std::pair<qint64, bool> bal    = Money::toCents(acc.balance.value_or(""));
    const std::pair<qint64, bool> credit = Money::toCents(amount);

    if (!credit.second) //отрицательный кредит допустим, как и нулеовй
    {
        errorResponceTemplate(ctx.jsonResponce, QStringLiteral("Credit sum is empty or incorrect"));
        return;
    }

    if (!bal.second)    //нулевой баланс допустим
    {
        errorResponceTemplate(ctx.jsonResponce, QStringLiteral("Incorrect balance in account"));
        return;
    }

    const qint64 newBal = bal.first + credit.first;
    if (accRepo->updateBalance(acc.id, Money::fromCents(newBal)))
    {
        Models::Transaction t;
        t.account_id = acc.id;
        t.amount     = amount;
        t.currency   = acc.currency;
        t.type       = QStringLiteral("credit");
        t.status     = QStringLiteral("done");

        const std::optional<Models::Transaction> created = txRepo->addTransaction(t);
        if (created.has_value())
            successResponce(ctx.jsonResponce, created.value());
        else
            errorResponceTemplate(ctx.jsonResponce, QStringLiteral("Failed to create transaction"));
    }
    else
        errorResponceTemplate(ctx.jsonResponce, QStringLiteral("Failed to update balances"));
}
