#ifndef ENDPOINT_TRANSACTIONLIST_H
#define ENDPOINT_TRANSACTIONLIST_H

#include "iendpoint.h"
#include <QJsonArray>
#include "../context/messagecontext.h"
#include "../common/constants.h"
#include "../common/serializers.h"
#include "../database/models/transaction.h"

namespace Endpoints {

class TransactionList : public IEndpoint
{
    void createTrListSuccessResponce(QJsonObject& responce, const QList<Models::Transaction> transactions)
    {
        QJsonArray arr;
        for (const auto &tx : transactions)
        {
            QJsonObject obj;
            serialize(obj, tx);
            arr.append(obj);
        }
        using namespace Common;
        responce[toStr(JsonField::Type)] = toStr(ProtocolType::TrList);
        responce[toStr(JsonField::Result)] = true;
        responce[toStr(JsonField::TrArr)]  = arr;
    }

    void CreateTrListErrorResponce(QJsonObject& responce, QString reason)
    {
        using namespace Common;
        responce[toStr(JsonField::Type)]   = toStr(ProtocolType::TrList);
        responce[toStr(JsonField::Result)] = false;
        responce[toStr(JsonField::Reason)] = reason;
    }
public:
    TransactionList() = default;
    void invoke(MessageContext& ctx) override;
};

}

#endif // ENDPOINT_TRANSACTIONLIST_H

