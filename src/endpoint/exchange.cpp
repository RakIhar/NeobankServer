#include "exchange.h"
#include "../common/converters.h"

void Endpoints::ExchangeRates::privateInvoke(MessageContext &ctx)
{
    //проверки валют
    const QJsonObject req   = ctx.jsonRequest;
    const QString fromS     = req.value("from").toString().trimmed();
    const QString toS       = req.value("to").toString().trimmed();

    Enums::Currency fromCur = Enums::fromStr(fromS, Enums::Currency::BYN).first;
    Enums::Currency toCur   = Enums::fromStr(toS, Enums::Currency::BYN).first;

    if (!exchange) {
        errorResponceTemplate(ctx.jsonResponce, "Exchange service unavailable");
        return;
    }

    ExchangeData ex = exchange->get(fromCur, toCur);

    successResponceTemplate(ctx.jsonResponce);
    QJsonObject obj;
    obj["from"] = Enums::toStr(ex.from);
    obj["to"] = Enums::toStr(ex.to);
    obj["rate"] = ex.exchangeRate;
    obj[toStr(JsonField::ExchangeRate)] = ex.exchangeRateStr;
    ctx.jsonResponce[toStr(JsonField::Obj)] = obj;
}
