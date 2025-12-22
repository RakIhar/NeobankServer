#ifndef ENDPOINT_TRANSACTIONLIST_H
#define ENDPOINT_TRANSACTIONLIST_H
#include "iendpoint.h"
#include <QJsonArray>
#include "../common/serializers.h"
#include "../database/repositories/transactionrepository.h"

namespace Endpoints
{
class TransactionList : public IEndpoint
{
    void successResponce(QJsonObject& responce, const QList<Models::Transaction> transactions,
                         const int limit, const int page, const int total)
    {
        successResponceTemplate(responce);
        QJsonArray arr;
        for (const auto &tx : transactions)
        {
            QJsonObject obj;
            serialize(obj, tx);
            arr.append(obj);
        }
        responce[toStr(JsonField::TrArr)]  = arr;
        responce[toStr(JsonField::Limit)] = limit;
        responce[toStr(JsonField::Page)]  = page;
        responce[toStr(JsonField::Count)] = total;
    }

    bool init(MessageContext& ctx) override
    {
        if (txRepo != nullptr)
            return true;

        txRepo = static_cast<Database::TransactionRepository*>(
            ctx.services.getRaw(typeid(Database::TransactionRepository).hash_code()));

        if (txRepo != nullptr)
            return true;
        return false;
    }
    Database::TransactionRepository* txRepo = nullptr;
public:
    ProtocolType prType() const override { return ProtocolType::TrList; };
    QString name() const override { return "TransactionList"; }
    void privateInvoke(MessageContext& ctx) override;
};
}

#endif // ENDPOINT_TRANSACTIONLIST_H

