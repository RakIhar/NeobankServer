#ifndef ACCOUNTCREATE_H
#define ACCOUNTCREATE_H

#include "iendpoint.h"
#include "../context/messagecontext.h"
#include "../database/models/account.h"
#include "../common/constants.h"

namespace Endpoints {

class AccountCreate : public IEndpoint {
    void createAccCreateSuccessResponce(QJsonObject& responce, Models::Account acc)
    {
        using namespace Common;
        QJsonObject accObj;
        accObj[toStr(JsonField::AccountId)] = acc.id;
        accObj[toStr(JsonField::Iban)]      = acc.iban;
        accObj[toStr(JsonField::Balance)]   = acc.balance;
        accObj[toStr(JsonField::Currency)]  = acc.currency;
        accObj[toStr(JsonField::Status)]    = acc.status;
        accObj[toStr(JsonField::CreatedAt)] = acc.created_at.toString(Qt::ISODate);
        responce[toStr(JsonField::Type)]    = toStr(ProtocolType::AccCreate);
        responce[toStr(JsonField::Result)]  = true;
        responce[toStr(JsonField::AccObj)]  = accObj;
    }

    void createAccCreateErrorResponce(QJsonObject& responce, QString reason)
    {
        using namespace Common;
        responce[toStr(JsonField::Type)]   = toStr(ProtocolType::AccCreate);
        responce[toStr(JsonField::Result)] = false;
        responce[toStr(JsonField::Reason)] = reason;
    }
public:
    void invoke(MessageContext &ctx) override;
};

}

#endif // ACCOUNTCREATE_H
