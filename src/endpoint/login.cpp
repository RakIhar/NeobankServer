#include "login.h"

void Endpoints::Login::privateInvoke(MessageContext &ctx)
{
    QJsonObject& request = ctx.jsonRequest;

    QString username = request.value(toStr(JsonField::Username)).toString();
    QString password = request.value(toStr(JsonField::Password)).toString();
    ctx.connection.userAgent = "";
    ctx.connection.ipAddress = "";
    std::optional<Models::AuthSession> sessionOpt = authService->authenticate(username, password, ctx.connection.ipAddress, ctx.connection.userAgent);
    if (sessionOpt.has_value())
    {
        Models::AuthSession authSession = sessionOpt.value();
        ctx.session.session_id = authSession.id;
        ctx.session.isAvailable = true;

        successResponce(ctx.jsonResponce, authSession.id, authSession.token);
        qDebug() << "User authenticated:" << username;
    }
    else
    {
        errorResponceTemplate(ctx.jsonResponce, "Invalid username or password");
        qDebug() << "Authentication failed for:" << username;
    }
}
