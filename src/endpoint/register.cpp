#include "register.h"
#include "../service/authentification.h"

using namespace Endpoints;
using namespace Common;

void Register::invoke(MessageContext &ctx)
{
    if (!ctx.isAborted)
    try
    {
        qDebug() << "[Register endpoint] enter";

        Services::Authentification* authService = static_cast<Services::Authentification*>(
                    ctx.services.getRaw(typeid(Services::Authentification).hash_code()));

        if (authService)
        {
            QJsonObject& request   = ctx.jsonRequest;
            const QString username = request.value(toStr(JsonField::Username)).toString();
            const QString password = request.value(toStr(JsonField::Password)).toString();
            const QString email    = request.value(toStr(JsonField::Email)).toString();
            const QString phone    = request.value(toStr(JsonField::Phone)).toString();

            if (!username.trimmed().isEmpty() && !password.isEmpty())
            {
                std::optional<Models::User> userOpt = authService->registerUser(username, password, email, phone);
                if (userOpt.has_value())
                    createRegisterSuccessResponce(ctx.jsonResponce);
                else
                {
                    createRegisterErrorResponce(ctx.jsonResponce, QStringLiteral("Registration failed"));
                    qDebug() << "[Register endpoint] Registration failed for:" << username;
                }
            }
            else
                createRegisterErrorResponce(ctx.jsonResponce, QStringLiteral("Username or password is empty"));
        }
        else
        {
            qDebug() << "[Register endpoint] Auth service unavailable";
            ctx.abort();
        }

        qDebug() << "[Register endpoint] exit";
    }
    catch (...)
    {
        qDebug() << "[Register endpoint] abort";
        ctx.abort();
    }
}

