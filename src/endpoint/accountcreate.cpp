#include "accountcreate.h"
#include <QJsonObject>
#include <QJsonArray>
#include "../database/repositories/accountrepository.h"
#include "../common/constants.h"
#include <QRandomGenerator>

using namespace Endpoints;
using namespace Database;
using namespace Common;

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

void AccountCreate::invoke(MessageContext &ctx)
{
    if (!ctx.isAborted)
    try {
        qDebug() << "[AccountCreate endpoint] enter";

        ctx.jsonResponce[toStr(JsonField::Type)] = toStr(ProtocolType::AccCreate);

        auto *repo = static_cast<AccountRepository*>(
            ctx.services.getRaw(typeid(AccountRepository).hash_code()));

        if (repo)
        {
            if (ctx.session.isAvailable && ctx.user.user_id)
            {
                QJsonObject& request   = ctx.jsonRequest;
                const QString currency = request.value(toStr(JsonField::Currency)).toString().trimmed();

                Models::Account acc;
                acc.user_id  = ctx.user.user_id;
                acc.currency = currency;
                acc.iban     = generateFakeIban(); // бд требует уникальный iban

                const std::optional<Models::Account> accOpt = repo->create(acc);
                if (accOpt.has_value())
                    createAccCreateSuccessResponce(ctx.jsonResponce, accOpt.value());
                else
                    createAccCreateErrorResponce(ctx.jsonResponce, QStringLiteral("Failed to create account"));
            }
            else
                createAccCreateErrorResponce(ctx.jsonResponce, QStringLiteral("Unauthorized"));
        }
        else
            createAccCreateErrorResponce(ctx.jsonResponce, QStringLiteral("AccountRepository unavailable"));

        qDebug() << "[AccountCreate endpoint] exit";
    }
    catch (...)
    {
        qDebug() << "[AccountCreate endpoint] abort";
        ctx.abort();
    }
}
