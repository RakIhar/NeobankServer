#ifndef TRANSACTIONBEFORE_H
#define TRANSACTIONBEFORE_H
#include "iendpoint.h"
#include "../common/serializers.h"
#include "../service/transactionservice.h"

namespace Endpoints
{
class TransactionBefore : public IEndpoint
{
    void responce(QJsonObject& responce, BeforeTransferInfo info) //нестандартный протокол
    {
        QJsonObject obj;
        serialize(obj, info);
        responce[toStr(JsonField::Type)]     = toStr(ProtocolType::TrBefore);
        responce[toStr(JsonField::TrObj)]    = obj;
    }

    bool init(MessageContext &ctx) override
    {
        if (svc != nullptr)
            return true;

        svc = static_cast<Services::TransactionService*>(
            ctx.services.getRaw(typeid(Services::TransactionService).hash_code()));

        if (svc != nullptr)
            return true;
        return false;
    }
    Services::TransactionService* svc = nullptr;

public:
    ProtocolType prType() const override { return ProtocolType::TrBefore; };
    QString name() const override { return "TransactionBefore"; }
    void privateInvoke(MessageContext &ctx) override;
};
}

#endif // TRANSACTIONBEFORE_H
