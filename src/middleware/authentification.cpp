#include "authentification.h"
#include "../database/models/user.h"
#include "../service/authentification.h"
#include "../service/session.h"
#include "../common/constants.h"

void Middlewares::Authentification::authenticate(MessageContext &ctx)
{
    Services::Authentification* authService = static_cast<Services::Authentification*>(
        ctx.services.getRaw(typeid(Services::Authentification).hash_code()));
    Services::Session* sessionService = static_cast<Services::Session*>(
        ctx.services.getRaw(typeid(Services::Session).hash_code()));

    if (!authService || !sessionService) {
        qDebug() << "[Authentification] Services not available";
        ctx.abort();
        return;
    }
    QJsonObject& request = ctx.jsonRequest;

    //установка sessionId и token
    ctx.session.session_id = {};
    const QString sessionIdStr = ctx.jsonRequest.value(Common::toStr(Common::JsonField::SessionId)).toString();
    ctx.session.session_id = QUuid::fromString(sessionIdStr);
    ctx.session.token = ctx.jsonRequest.value(Common::toStr(Common::JsonField::Token)).toString();

    std::optional<Models::AuthSession> sessionOpt;
    if (!ctx.session.session_id.isNull()) {
        sessionOpt = authService->validateSession(ctx.session.session_id);
    } else if (!ctx.session.token.isEmpty()) {
        sessionOpt = authService->validateSessionByToken(ctx.session.token);
    }
    //установка session_id, user_id. установка логина
    if (sessionOpt.has_value()) {
        ctx.session.isAvailable = true;
        ctx.session.session_id = sessionOpt.value().id;

        std::optional<Models::User> userOpt = authService->getUserBySession(ctx.session.session_id);
        if (userOpt.has_value()) {
            Models::User dbUser = userOpt.value();
            ctx.user.user_id = dbUser.id;
            ctx.user.username = dbUser.username;
            //мейби добавить полей в User
        }

        qDebug() << "[Authentification] Session validated:" << ctx.session.session_id.toString();
    } else {
        qDebug() << "[Authentification] Session not found or invalid";
        ctx.session.isAvailable = false;
    }
}
