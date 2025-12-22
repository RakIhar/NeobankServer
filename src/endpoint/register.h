#ifndef ENDPOINT_REGISTER_H
#define ENDPOINT_REGISTER_H
#include "iendpoint.h"
#include "../common/constants.h"
#include "../service/authentification.h"

namespace Endpoints
{
class Register : public IEndpoint
{
    void successResponce(QJsonObject& responce)
    {
        using namespace Common;
        responce[toStr(JsonField::Type)]   = toStr(ProtocolType::Register);
        responce[toStr(JsonField::Result)] = true;
    }

    bool init(MessageContext& ctx) override
    {
        if (authService != nullptr )
            return true;

        authService = static_cast<Services::Authentification*>(
            ctx.services.getRaw(typeid(Services::Authentification).hash_code()));

        return authService != nullptr;
    };

    Services::Authentification* authService = nullptr;

public:
    ProtocolType prType() const override { return ProtocolType::Register; };
    QString name() const override { return "Register"; }
    void privateInvoke(MessageContext& ctx) override;
};
}

#endif // REGISTER_H
