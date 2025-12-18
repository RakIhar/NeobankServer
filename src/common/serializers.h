#ifndef SERIALIZERS_H
#define SERIALIZERS_H
#include <QJsonObject>
#include "../database/models/transaction.h"
#include "constants.h"

inline void serialize(QJsonObject& obj, Models::Transaction tx) //добавляет напрямую в объект, чтобы ничего не ломать
{
    using namespace Common;
    obj[toStr(JsonField::TransactionId)]  = tx.id;
    obj[toStr(JsonField::AccountId)]      = tx.account_id;
    obj[toStr(JsonField::CounterpartyId)] = tx.counterparty_account_id.has_value()
                                            ? QJsonValue(tx.counterparty_account_id.value())
                                            : QJsonValue::Null;
    obj[toStr(JsonField::Amount)]         = tx.amount;
    obj[toStr(JsonField::Currency)]       = tx.currency;
    obj[toStr(JsonField::Type)]           = tx.type;
    obj[toStr(JsonField::Reason)]         = tx.description.has_value()
                                            ? QJsonValue(tx.description.value())
                                            : QJsonValue::Null;
    obj[toStr(JsonField::Status)]         = tx.status.has_value()
                                            ? QJsonValue(tx.status.value())
                                            : QJsonValue::Null;
    obj[toStr(JsonField::CreatedAt)]      = tx.created_at.has_value()
                                            ? QJsonValue(tx.created_at.value().toString(Qt::ISODate))
                                            : QJsonValue::Null;
}

#endif // SERIALIZERS_H
