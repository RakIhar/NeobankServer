#include "accountcreate.h"
#include <QRandomGenerator>

QString generateFakeIban(const QString& countryCode = "BY") {
    QString iban = countryCode;
    iban += QString::number(QRandomGenerator::global()->bounded(10, 99));
    iban += "NEOB";
    qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
    iban += QString::number(timestamp);
    while(iban.length() < 28)
        iban += QString::number(QRandomGenerator::global()->bounded(0, 10));
    return iban;
}

//TODO: вынести в AccountService
void Endpoints::AccountCreate::privateInvoke(MessageContext &ctx)
{
    QJsonObject& request   = ctx.jsonRequest;
    const QString currency = request.value(toStr(JsonField::Currency)).toString().trimmed();

    Models::Account acc;
    acc.user_id  = ctx.user.user_id;
    acc.currency = currency;
    acc.iban     = generateFakeIban(); // бд требует уникальный iban

    const std::optional<Models::Account> accOpt = repo->create(acc);
    if (accOpt.has_value())
        successResponce(ctx.jsonResponce, accOpt.value());
    else
        errorResponceTemplate(ctx.jsonResponce, QStringLiteral("Failed to create account"));
}
