#include "login.h"
#include "../service/authentification.h"
#include "../service/session.h"
using namespace Common;

void Endpoints::Login::invoke(MessageContext &ctx)
{
    if (!ctx.isAborted)
    try
    {
        qDebug() << "[Login endpoint] enter";

        Services::Authentification* authService = static_cast<Services::Authentification*>(
            ctx.services.getRaw(typeid(Services::Authentification).hash_code()));
        Services::Session* sessionService = static_cast<Services::Session*>(
            ctx.services.getRaw(typeid(Services::Session).hash_code()));

        if (authService && sessionService)
        {
            QJsonObject& request = ctx.jsonRequest;

            QString username = request.value(toStr(JsonField::Username)).toString();
            QString password = request.value(toStr(JsonField::Password)).toString();
            //CHECK: возможно transport layer будет возвращать user_agent и ip_address
            //сейчас это заглушки
            QString ipAddress = ctx.items.value("ip_address", "").toString();
            QString userAgent = ctx.items.value("user_agent", "").toString();

            std::optional<Models::AuthSession> sessionOpt = authService->authenticate(username, password, ipAddress, userAgent);
            if (sessionOpt.has_value())
            {
                Models::AuthSession authSession = sessionOpt.value();
                ctx.session.session_id = authSession.id;
                ctx.session.isAvailable = true;

                createLoginSuccessResponce(ctx.jsonResponce, authSession.id, authSession.token);
                qDebug() << "[Login endpoint] User authenticated:" << username;
            }
            else
            {
                createLoginErrorResponce(ctx.jsonResponce);
                qDebug() << "[Login endpoint] Authentication failed for:" << username;
            }
        }
        else
        {
            qDebug() << "[Login endpoint] Services not available";
            ctx.abort();
        }

        qDebug() << "[Login endpoint] exit";
    }
    catch (...)
    {
        qDebug() << "[Login endpoint] abort";
        ctx.abort();
    }
}
