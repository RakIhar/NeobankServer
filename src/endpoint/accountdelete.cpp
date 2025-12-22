#include "accountdelete.h"

void Endpoints::AccountDelete::privateInvoke(MessageContext &ctx)
{
    QJsonObject& request   = ctx.jsonRequest;
    qint64 acc_id = ctx.jsonRequest.value(toStr(JsonField::AccountId)).toVariant().toLongLong();
    auto res = repo->deleteById(acc_id);
    if (res.first)
        successResponceTemplate(ctx.jsonResponce);
    else
        errorResponceTemplate(ctx.jsonResponce, res.second);
}
