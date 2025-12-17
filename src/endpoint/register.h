#ifndef ENDPOINT_REGISTER_H
#define ENDPOINT_REGISTER_H

#include "iendpoint.h"
#include <QDebug>
#include <QJsonObject>
#include "../context/messagecontext.h"
#include "../common/constants.h"

namespace Endpoints {

class Register : public IEndpoint
{
    void createRegisterSuccessResponce(QJsonObject& responce)
    {
        using namespace Common;
        responce[toStr(JsonField::Type)]   = toStr(ProtocolType::Register);
        responce[toStr(JsonField::Result)] = true;
    }

    void createRegisterErrorResponce(QJsonObject& responce, const QString& reason)
    {
        using namespace Common;
        responce[toStr(JsonField::Type)]   = toStr(ProtocolType::Register);
        responce[toStr(JsonField::Result)] = false;
        responce[toStr(JsonField::Reason)] = reason;
    }
public:
    Register(){};

    void invoke(MessageContext& ctx) override;
};

}

#endif // REGISTER_H
