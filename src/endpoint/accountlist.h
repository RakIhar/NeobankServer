#ifndef ENDPOINT_ACCOUNTLIST_H
#define ENDPOINT_ACCOUNTLIST_H

#include "iendpoint.h"
#include <QJsonArray>
#include "../context/messagecontext.h"
#include "../common/constants.h"
#include "../common/serializers.h"
#include "../database/models/account.h"

namespace Endpoints {

class AccountList : public IEndpoint
{
    void createAccListSuccessResponce(QJsonObject& responce, const QList<Models::Account> accounts)
    {
        using namespace Common;

        QJsonArray arr;
        for (const auto &acc : accounts)
        {
            QJsonObject obj;
            serialize(obj, acc);
            arr.append(obj);
        }
        responce[toStr(JsonField::Type)] = toStr(ProtocolType::AccList);
        responce[toStr(JsonField::Result)] = true;
        responce[toStr(JsonField::AccArr)] = arr;
    }

    void CreateAccListErrorResponce(QJsonObject& responce, QString reason)
    {
        using namespace Common;
        responce[toStr(JsonField::Type)]   = toStr(ProtocolType::AccList);
        responce[toStr(JsonField::Result)] = false;
        responce[toStr(JsonField::Reason)] = reason;
    }
public:
    AccountList() = default;
    void invoke(MessageContext& ctx) override;
};

}

#endif // ENDPOINT_ACCOUNTLIST_H

