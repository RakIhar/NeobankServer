#include "accountlist.h"
#include "../database/repositories/accountrepository.h"

using namespace Endpoints;
using namespace Common;

void AccountList::invoke(MessageContext &ctx)
{
    if (!ctx.isAborted)
    try
    {
        qDebug() << "[AccountList endpoint] enter";

        Database::AccountRepository *repo = static_cast<Database::AccountRepository*>(
            ctx.services.getRaw(typeid(Database::AccountRepository).hash_code()));

        if (repo)
        {
            if (ctx.session.isAvailable && ctx.user.user_id)
                createAccListSuccessResponce(ctx.jsonResponce, repo->getByUser(ctx.user.user_id));
            else
                CreateAccListErrorResponce(ctx.jsonResponce, QStringLiteral("Unauthorized"));
        }
        else
            CreateAccListErrorResponce(ctx.jsonResponce, QStringLiteral("AccountRepository unavailable"));

        qDebug() << "[AccountList endpoint] exit";
    }
    catch (...)
    {
        qDebug() << "[AccountList endpoint] abort";
        ctx.abort();
    }
}
