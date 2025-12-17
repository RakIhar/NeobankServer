#include "transactioncreate.h"
#include <QJsonObject>
#include "../database/repositories/transactionrepository.h"
#include "../database/repositories/accountrepository.h"
#include "../common/constants.h"
#include "../common/converters.h"

using namespace Endpoints;
using namespace Database;
using namespace Common;

void TransactionCreate::fromSystem(MessageContext &ctx, qint64 toId)
{
    const std::optional<Models::Account> toAccOpt = accRepo->getById(toId);
    if (toAccOpt.has_value())
    {
        const QJsonObject request = ctx.jsonRequest;
        const QString amount      = request.value(toStr(JsonField::Amount)).toString();
        const QString currency    = request.value(toStr(JsonField::Currency)).toString();
        const QString description = request.value(toStr(JsonField::Reason)).toString();

        Models::Account toAcc = toAccOpt.value();
        const qint64 oldB     = Money::toCents(toAcc.balance);
        const qint64 add      = Money::toCents(amount);
        const qint64 newB     = oldB + add;

        if (accRepo->updateBalance(toAcc.id, Money::fromCents(newB)))
        {
            Models::Transaction t;
            t.account_id                = toAcc.id;
            t.counterparty_account_id   = 0;
            t.amount                    = amount;
            t.currency                  = currency;
            t.type                      = QStringLiteral("credit");
            t.description               = description;
            t.status                    = QStringLiteral("done");

            const auto created = txRepo->addTransaction(t);
            if (created.has_value())
                createTrCreateSuccessResponce(ctx.jsonResponce, created.value());
            else
                createTrCreateErrorResponce(ctx.jsonResponce, QStringLiteral("Failed to create transaction"));
        }
        else
            createTrCreateErrorResponce(ctx.jsonResponce, QStringLiteral("Failed to update balance"));
    }
    else
        createTrCreateErrorResponce(ctx.jsonResponce, QStringLiteral("Destination account not found"));
}

void TransactionCreate::betweenAccounts(MessageContext &ctx, qint64 fromId, qint64 toId)
{
    const QJsonObject request = ctx.jsonRequest;
    const QString amount      = request.value(toStr(JsonField::Amount)).toString();
    const QString currency    = request.value(toStr(JsonField::Currency)).toString();
    const QString description = request.value(toStr(JsonField::Reason)).toString();

    const std::optional<Models::Account> fromAccOpt = accRepo->getById(fromId);
    const std::optional<Models::Account> toAccOpt = accRepo->getById(toId);
    if (fromAccOpt.has_value() && toAccOpt.has_value())
    {
        Models::Account fromAcc = fromAccOpt.value();
        Models::Account toAcc   = toAccOpt.value();
        const qint64 fromBal    = Money::toCents(fromAcc.balance);
        const qint64 transfer   = Money::toCents(amount);
        if (transfer > 0 && fromBal >= transfer)
        {
            const qint64 newFrom = fromBal - transfer;
            const qint64 newTo = Money::toCents(toAcc.balance) + transfer;
            if (accRepo->updateBalance(fromAcc.id, Money::fromCents(newFrom)) &&
                accRepo->updateBalance(toAcc.id,   Money::fromCents(newTo)))
            {
                Models::Transaction tFrom;
                tFrom.account_id                = fromAcc.id;
                tFrom.counterparty_account_id   = toAcc.id;
                tFrom.amount                    = amount;
                tFrom.currency                  = currency;
                tFrom.type                      = QStringLiteral("debit");
                tFrom.description               = description;
                tFrom.status                    = QStringLiteral("done");

                Models::Transaction tTo;
                tTo.account_id                  = toAcc.id;
                tTo.counterparty_account_id     = fromAcc.id;
                tTo.amount                      = amount;
                tTo.currency                    = currency;
                tTo.type                        = QStringLiteral("credit");
                tTo.description                 = description;
                tTo.status                      = QStringLiteral("done");

                const std::optional<Models::Transaction> cFrom = txRepo->addTransaction(tFrom);
                const std::optional<Models::Transaction> cTo = txRepo->addTransaction(tTo);
                if (cFrom.has_value() && cTo.has_value())
                    createTrCreateSuccessResponce(ctx.jsonResponce, cTo.value());
                else
                    createTrCreateErrorResponce(ctx.jsonResponce, QStringLiteral("Failed to create transactions"));
            }
            else
                createTrCreateErrorResponce(ctx.jsonResponce, QStringLiteral("Failed to update balances"));
        }
        else
            createTrCreateErrorResponce(ctx.jsonResponce, QStringLiteral("Insufficient funds or invalid amount"));
    }
    else
        createTrCreateErrorResponce(ctx.jsonResponce, QStringLiteral("Account not found"));
}

bool TransactionCreate::initRepos(MessageContext &ctx)
{
    if (txRepo && accRepo)
        return true;

    txRepo = static_cast<TransactionRepository*>(
        ctx.services.getRaw(typeid(TransactionRepository).hash_code()));
    accRepo = static_cast<AccountRepository*>(
        ctx.services.getRaw(typeid(AccountRepository).hash_code()));

    if (txRepo && accRepo)
        return true;
    return false;
}

void TransactionCreate::invoke(MessageContext &ctx)
{
    if (!ctx.isAborted)
    try {
        qDebug() << "[TransactionCreate endpoint] enter";
        if (initRepos(ctx))
        {
            if (ctx.session.isAvailable && ctx.user.user_id)
            {
                const QJsonObject request = ctx.jsonRequest;
                const QVariant fromVar    = request.value(toStr(JsonField::FromAcc)).toVariant();
                const QVariant toVar      = request.value(toStr(JsonField::ToAcc)).toVariant();

                qint64 fromId     = fromVar.toLongLong();
                qint64 toId       = toVar.toLongLong();
                bool fromIsSystem = fromVar.toString() == "SYSTEM";

                if (fromIsSystem || fromId > 0)
                {
                    if (toId > 0)
                    {
                        if (fromIsSystem)
                            fromSystem(ctx, toId);
                        else
                            betweenAccounts(ctx, fromId, toId);
                    }
                    else
                        createTrCreateErrorResponce(ctx.jsonResponce, "Invalid to account");
                }
                else
                    createTrCreateErrorResponce(ctx.jsonResponce, "Invalid from account");
            }
            else
                createTrCreateErrorResponce(ctx.jsonResponce, QStringLiteral("Unauthorized"));
        }
        else
            createTrCreateErrorResponce(ctx.jsonResponce, QStringLiteral("Repositories unavailable"));
    }
    catch (...)
    {
        qDebug() << "[TransactionCreate endpoint] abort";
        ctx.abort();
    }
}
