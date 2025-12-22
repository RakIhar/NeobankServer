#include "transactionbefore.h"

void Endpoints::TransactionBefore::privateInvoke(MessageContext &ctx)
{
    const QJsonObject request = ctx.jsonRequest;
    const QString toVar       = request.value(toStr(JsonField::ToAcc)).toString();
    const QString amountStr   = request.value(toStr(JsonField::Amount)).toString();
    const QString fromVar     = request.value(toStr(JsonField::FromAcc)).toString();
    responce(ctx.jsonResponce, svc->getBeforeTransferInfo(toVar, amountStr, fromVar));
}
