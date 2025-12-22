#include "transactionlist.h"

void Endpoints::TransactionList::privateInvoke(MessageContext &ctx)
{
    const int limit = std::clamp(ctx.jsonRequest.value(toStr(JsonField::Limit)).toInt(50), 1, 200);
    const int page = std::max(0, ctx.jsonRequest.value(toStr(JsonField::Page)).toInt(0));
    QList<Models::Transaction> txs = txRepo->getRecentForUser(ctx.user.user_id, limit, page);
    int total = txRepo->getCountForUser(ctx.user.user_id);
    successResponce(ctx.jsonResponce, txs, limit, page, total);
}
