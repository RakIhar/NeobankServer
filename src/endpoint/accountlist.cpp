#include "accountlist.h"

void Endpoints::AccountList::privateInvoke(MessageContext &ctx)
{
    const int limit = std::clamp(ctx.jsonRequest.value(toStr(JsonField::Limit)).toInt(50), 1, 200);
    const int page = std::max(0, ctx.jsonRequest.value(toStr(JsonField::Page)).toInt(0));
    auto accs = repo->getByUser(ctx.user.user_id, limit, page);
    int total = repo->getCountForUser(ctx.user.user_id);
    successResponce(ctx.jsonResponce, accs, limit, page, total);
}
