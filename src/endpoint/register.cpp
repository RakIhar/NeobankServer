#include "register.h"

void Endpoints::Register::privateInvoke(MessageContext &ctx)
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
            successResponce(ctx.jsonResponce);
        else
        {
            errorResponceTemplate(ctx.jsonResponce, QStringLiteral("Registration failed"));
            qDebug() << "Registration failed for:" << username;
        }
    }
    else
        errorResponceTemplate(ctx.jsonResponce, QStringLiteral("Username or password is empty"));
}

