#include "authentification.h"
#include "../database/models/user.h"

void Middlewares::Authentification::authenticate(MessageContext &ctx, Services::Authentification *authService, QJsonObject &request)
{
    QUuid sessionId;
    QString token;
    if (request.contains(toStr(Common::JsonField::SessionId))) {
        QString sessionIdStr = request[toStr(Common::JsonField::SessionId)].toString();
        sessionId = QUuid::fromString(sessionIdStr);
    }
    if (request.contains(toStr(Common::JsonField::Token))) {
        token = request[toStr(Common::JsonField::Token)].toString();
    }

    std::optional<Models::AuthSession> sessionOpt;
    if (!sessionId.isNull()) {
        sessionOpt = authService->validateSession(sessionId);
    } else if (!token.isEmpty()) {
        sessionOpt = authService->validateSessionByToken(token);
    }

    if (sessionOpt.has_value()) { //установка id для user и session. установка логина
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

        qDebug() << "[Authentification] Session validated:" << authSession.id.toString();
    } else {
        qDebug() << "[Authentification] Session not found or invalid";
        ctx.session.isAvailable = false;
    }
}
