#ifndef ENDPOINT_LOGIN_H
#define ENDPOINT_LOGIN_H

#include "iendpoint.h"
#include <QDebug>
#include "../context/messagecontext.h"
#include "../common/constants.h"

namespace Endpoints {

class Login : public IEndpoint
{   
    void createLoginSuccessResponce(QJsonObject& responce, QUuid sessionId, QString token)
    {
        using namespace Common;
        responce[toStr(JsonField::Type)]      = toStr(ProtocolType::Login);
        responce[toStr(JsonField::SessionId)] = sessionId.toString(QUuid::WithoutBraces);
        responce[toStr(JsonField::Token)]     = token;
        responce[toStr(JsonField::Result)]    = true;
    }

    void createLoginErrorResponce(QJsonObject& responce)
    {
        using namespace Common;
        responce[toStr(JsonField::Type)]   = toStr(ProtocolType::Login);
        responce[toStr(JsonField::Result)] = false;
        responce[toStr(JsonField::Reason)] = "Invalid username or password";
    }

public:
    Login(){};

    void invoke(MessageContext& ctx) override;;
};

}

#endif // LOGIN_H
