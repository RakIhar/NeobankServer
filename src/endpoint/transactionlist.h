#ifndef ENDPOINT_TRANSACTIONLIST_H
#define ENDPOINT_TRANSACTIONLIST_H

#include "iendpoint.h"
#include <QJsonArray>
#include "../context/messagecontext.h"
#include "../common/constants.h"
#include "../database/models/transaction.h"

namespace Endpoints {

class TransactionList : public IEndpoint
{
    void createTrListSuccessResponce(QJsonObject& responce, const QList<Models::Transaction> transactions)
    {
        using namespace Common;
        QJsonArray arr;
        for (const auto &tx : transactions)
        {
            QJsonObject obj;
            obj[toStr(JsonField::TransactionId)]  = tx.id;
            obj[toStr(JsonField::AccountId)]      = tx.account_id;
            obj[toStr(JsonField::CounterpartyId)] = tx.counterparty_account_id.has_value()
                                                        ? QJsonValue(tx.counterparty_account_id.value())
                                                        : QJsonValue::Null;
            obj[toStr(JsonField::Amount)]         = tx.amount;
            obj[toStr(JsonField::Currency)]       = tx.currency;
            obj[toStr(JsonField::Type)]           = tx.type;
            obj[toStr(JsonField::Reason)]         = tx.description;
            obj[toStr(JsonField::Status)]         = tx.status;
            obj[toStr(JsonField::CreatedAt)]      = tx.created_at.toString(Qt::ISODate);
            arr.append(obj);
        }
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

