#include "transactionlist.h"
#include <algorithm>
#include "../database/repositories/transactionrepository.h"
using namespace Endpoints;
using namespace Common;

void TransactionList::invoke(MessageContext &ctx)
{
    if (!ctx.isAborted)
    try
    {
        qDebug() << "[TransactionList endpoint] enter";

        auto *repo = static_cast<Database::TransactionRepository*>(
            ctx.services.getRaw(typeid(Database::TransactionRepository).hash_code()));

        if (repo)
        {
            if (ctx.session.isAvailable && ctx.user.user_id)
            {
                const int limit = std::clamp(ctx.jsonRequest.value(toStr(JsonField::Limit)).toInt(50), 1, 200);
                createTrListSuccessResponce(ctx.jsonResponce, repo->getRecentForUser(ctx.user.user_id, limit));
            }
            else
                CreateTrListErrorResponce(ctx.jsonResponce, QStringLiteral("Unauthorized"));
        }
        else
            CreateTrListErrorResponce(ctx.jsonResponce, QStringLiteral("TransactionRepository unavailable"));

        qDebug() << "[TransactionList endpoint] exit";
    }
    catch (...)
    {
        qDebug() << "[TransactionList endpoint] abort";
        ctx.abort();
    }
}
