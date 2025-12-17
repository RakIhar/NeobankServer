#ifndef TRANSACTIONCREATE_H
#define TRANSACTIONCREATE_H

#include "iendpoint.h"
#include "../context/messagecontext.h"
#include "../database/models/transaction.h"
#include "../common/constants.h"
#include "../database/repositories/transactionrepository.h"
#include "../database/repositories/accountrepository.h"
namespace Endpoints {

class TransactionCreate : public IEndpoint {
    void fromSystem(MessageContext &ctx, qint64 toId);
    void betweenAccounts(MessageContext &ctx, qint64 fromId, qint64 toId);

    void createTrCreateSuccessResponce(QJsonObject& responce, Models::Transaction tr)
    {
        using namespace Common;
        QJsonObject obj;
        obj[toStr(JsonField::TransactionId)] = tr.id;
        obj[toStr(JsonField::AccountId)]     = tr.account_id;
        obj[toStr(JsonField::Amount)]        = tr.amount;
        obj[toStr(JsonField::Currency)]      = tr.currency;
        obj[toStr(JsonField::Type)]          = tr.type;
        obj[toStr(JsonField::Reason)]        = tr.description;
        obj[toStr(JsonField::Status)]        = tr.status;
        obj[toStr(JsonField::CreatedAt)]     = tr.created_at.toString(Qt::ISODate);
        responce[toStr(JsonField::Type)]     = toStr(ProtocolType::TrCreate);
        responce[toStr(JsonField::Result)]   = true;
        responce[toStr(JsonField::TrObj)]    = obj;
    }

    void createTrCreateErrorResponce(QJsonObject& responce, QString reason)
    {
        using namespace Common;
        responce[toStr(JsonField::Type)]   = toStr(ProtocolType::TrCreate);
        responce[toStr(JsonField::Result)] = false;
        responce[toStr(JsonField::Reason)] = reason;
    }

    bool initRepos(MessageContext &ctx);
    Database::AccountRepository *accRepo = nullptr;
    Database::TransactionRepository *txRepo = nullptr;
public:
    void invoke(MessageContext &ctx) override;
};

}

#endif // TRANSACTIONCREATE_H
