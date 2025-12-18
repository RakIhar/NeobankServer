#ifndef SERIALIZERS_H
#define SERIALIZERS_H
#include <QJsonObject>
#include "../database/models/transaction.h"
#include "../database/models/account.h"
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

inline void serialize(QJsonObject& accObj, Models::Account acc)
{
    using namespace Common;
    accObj[toStr(JsonField::AccountId)] = acc.id;
    accObj[toStr(JsonField::UserId)]    = acc.user_id;
    accObj[toStr(JsonField::Iban)]      = acc.iban.has_value()
                                          ? QJsonValue(acc.iban.value())
                                          : QJsonValue::Null;
    accObj[toStr(JsonField::Balance)]   = acc.balance.has_value()
                                          ? QJsonValue(acc.balance.value())
                                          : QJsonValue::Null;
    accObj[toStr(JsonField::Currency)]  = acc.currency;
    accObj[toStr(JsonField::Status)]    = acc.status.has_value()
                                          ? QJsonValue(acc.status.value())
                                          : QJsonValue::Null;
    accObj[toStr(JsonField::CreatedAt)] = acc.created_at.has_value()
                                          ? QJsonValue(acc.created_at.value().toString(Qt::ISODate))
                                          : QJsonValue::Null;
    accObj[toStr(JsonField::UpdatedAt)] = acc.updated_at.has_value()
                                          ? QJsonValue(acc.updated_at.value().toString(Qt::ISODate))
                                          : QJsonValue::Null;
}

#endif // SERIALIZERS_H
