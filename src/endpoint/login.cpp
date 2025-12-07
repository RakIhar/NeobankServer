#include "login.h"

void Endpoints::Login::invoke(MessageContext &ctx)
{
    if (!ctx.isAborted)
    {
        try
        {
            qDebug() << "[Login endpoint] enter";

            Services::Authentification* authService = static_cast<Services::Authentification*>(
                ctx.services.getRaw(typeid(Services::Authentification).hash_code()));
            Services::Session* sessionService = static_cast<Services::Session*>(
                ctx.services.getRaw(typeid(Services::Session).hash_code()));

            if (!authService || !sessionService) {
                qDebug() << "[Login endpoint] Services not available";
                ctx.abort();
                return;
            }
            QJsonObject& request = ctx.jsonRequest;

            QString username = request.value(toStr(Common::JsonField::Username)).toString();
            QString password = request.value(toStr(Common::JsonField::Password)).toString();

            QString ipAddress = ctx.items.value("ip_address", "").toString();//наверное убрать оба
            QString userAgent = ctx.items.value("user_agent", "").toString();

            std::optional<Models::AuthSession> sessionOpt = authService->authenticate(username, password, ipAddress, userAgent);
            if (sessionOpt.has_value()) {
                Models::AuthSession authSession = sessionOpt.value();
                ctx.session.sessionId = authSession.id;
                ctx.session.isAvailable = true;

                std::optional<Models::User> userOpt = authService->getUserBySession(authSession.id);
                if (userOpt.has_value()) {
                    Models::User dbUser = userOpt.value();
                    ctx.user.id = dbUser.id;
                    ctx.user.username = dbUser.username;
                    //мейби добавить полей в User
                }

                createLoginSuccessResponce(ctx.jsonResponce, authSession.id, authSession.token);

                qDebug() << "[Login endpoint] User authenticated:" << username;
            } else {
                createLoginErrorResponce(ctx.jsonResponce);

                qDebug() << "[Login endpoint] Authentication failed for:" << username;
            }

            qDebug() << "[Login endpoint] exit";
        }
        catch (...)
        {
            qDebug() << "[Login endpoint] abort";
            ctx.abort();
        }
    }
}
