#include "transactioncreate.h"

void Endpoints::TransactionCreate::privateInvoke(MessageContext &ctx)
{
    const QJsonObject request = ctx.jsonRequest;
    const QString amount      = request.value(toStr(JsonField::Amount)).toString();
    const QString description = request.value(toStr(JsonField::Descr)).toString();
    const QVariant fromVar    = request.value(toStr(JsonField::FromAcc)).toVariant();
    const QVariant toVar      = request.value(toStr(JsonField::ToAcc)).toVariant();

    QString err;
    std::optional<Models::Transaction> res = svc->createTransfer(fromVar, toVar, amount, description, err);
    if (res.has_value()) {
        successResponce(ctx.jsonResponce, res.value());
    } else {
        errorResponceTemplate(ctx.jsonResponce, err);
    }
}
