#ifndef ENDPOINT_LOGIN_H
#define ENDPOINT_LOGIN_H
#include "iendpoint.h"
#include "../common/constants.h"
#include "../service/authentification.h"
#include "../service/session.h"

namespace Endpoints
{
class Login : public IEndpoint
{
    void successResponce(QJsonObject& responce, QUuid sessionId, QString token)
    {
        successResponceTemplate(responce);
        responce[toStr(JsonField::SessionId)] = sessionId.toString(QUuid::WithoutBraces);
        responce[toStr(JsonField::Token)]     = token;
    }

    bool init(MessageContext& ctx) override
    {
        if (authService != nullptr && sessionService != nullptr)
            return true;

        authService = static_cast<Services::Authentification*>(
            ctx.services.getRaw(typeid(Services::Authentification).hash_code()));
        sessionService = static_cast<Services::Session*>(
            ctx.services.getRaw(typeid(Services::Session).hash_code()));

        return authService != nullptr && sessionService != nullptr;
    };

    Services::Authentification* authService = nullptr;
    Services::Session* sessionService = nullptr;
public:
    ProtocolType prType() const override { return ProtocolType::Login; };
    QString name() const override { return "Login"; }
    void privateInvoke(MessageContext& ctx) override;
};
}

#endif // LOGIN_H
