#ifndef CREDITCREATE_H
#define CREDITCREATE_H
#include "iendpoint.h"
#include "../database/repositories/transactionrepository.h"
#include "../database/repositories/accountrepository.h"
#include "../common/serializers.h"

namespace Endpoints
{
class CreditCreate : public IEndpoint
{
    void handleTransaction(MessageContext &ctx);

    void successResponce(QJsonObject& responce, Models::Transaction tr)
    {
        successResponceTemplate(responce);
        QJsonObject obj;
        serialize(obj, tr);
        responce[toStr(JsonField::TrObj)] = obj;
    }

    bool init(MessageContext &ctx) override
    {
        if (txRepo != nullptr && accRepo != nullptr)
            return true;

        txRepo = static_cast<Database::TransactionRepository*>(
            ctx.services.getRaw(typeid(Database::TransactionRepository).hash_code()));
        accRepo = static_cast<Database::AccountRepository*>(
            ctx.services.getRaw(typeid(Database::AccountRepository).hash_code()));

        if (txRepo != nullptr && accRepo != nullptr)
            return true;
        return false;
    }
    Database::TransactionRepository* txRepo = nullptr;
    Database::AccountRepository* accRepo = nullptr;
public:
    ProtocolType prType() const override { return ProtocolType::CreditCreate; };
    QString name() const override { return "CreditCreate"; }
    void privateInvoke(MessageContext &ctx) override;
};
}

#endif // CREDITCREATE_H
