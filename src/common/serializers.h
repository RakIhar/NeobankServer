#ifndef SERIALIZERS_H
#define SERIALIZERS_H
#include <QJsonObject>
#include "../database/models/transaction.h"
#include "../database/models/account.h"
#include "constants.h"

//std::optional - поле может отсутствовать вовсе

inline void serialize(QJsonObject& obj, Models::Transaction tx) //добавляет напрямую в объект, чтобы ничего не ломать
{
    using namespace Common;
    obj[toStr(JsonField::TransactionId)] = tx.id;
    obj[toStr(JsonField::AccountId)]     = tx.account_id;

    if (tx.counterparty_account_id.has_value())
        obj[toStr(JsonField::CounterpartyId)] = tx.counterparty_account_id.value();

    obj[toStr(JsonField::Amount)]        = tx.amount;
    obj[toStr(JsonField::Currency)]      = tx.currency;
    obj[toStr(JsonField::Type)]          = tx.type;

    if (tx.description.has_value())
        obj[toStr(JsonField::Descr)]     = tx.description.value();

    if (tx.status.has_value())
        obj[toStr(JsonField::Status)]    = tx.status.value();

    if (tx.created_at.has_value())
    obj[toStr(JsonField::CreatedAt)]     = tx.created_at.value().toString(Qt::ISODate);

    if (tx.metadata.has_value()) {
        obj[toStr(JsonField::Metadata)]  = tx.metadata.value();
    }
}

inline void serialize(QJsonObject& obj, Models::Account acc)
{
    using namespace Common;
    obj[toStr(JsonField::AccountId)] = acc.id;
    obj[toStr(JsonField::UserId)]    = acc.user_id;
    obj[toStr(JsonField::Currency)]  = acc.currency;

    if (acc.iban.has_value())
        obj[toStr(JsonField::Iban)]      = acc.iban.value();

    if (acc.balance.has_value())
        obj[toStr(JsonField::Balance)]   = acc.balance.value();

    if (acc.status.has_value())
        obj[toStr(JsonField::Status)]    = acc.status.value();

    if (acc.created_at.has_value())
        obj[toStr(JsonField::CreatedAt)] = acc.created_at.value().toString(Qt::ISODate);

    if (acc.updated_at.has_value())
        obj[toStr(JsonField::UpdatedAt)] = acc.updated_at.value().toString(Qt::ISODate);
}

inline void serialize(QJsonObject& obj, BeforeTransferInfo info)
{
    using namespace Common;
    obj[toStr(JsonField::Result)]       = info.isAllowed;
    obj[toStr(JsonField::Error)]        = info.error;
    obj[toStr(JsonField::Comission)]    = info.comission;
    obj[toStr(JsonField::ExchangeRate)] = info.exchangeRate;
    obj[toStr(JsonField::Amount)]       = info.resultAmount;

    if(info.to_acc.has_value())
    {
        QJsonObject accObj;
        serialize(accObj, info.to_acc.value());
        obj[toStr(JsonField::AccObj)]   = accObj;
    }
}

#endif // SERIALIZERS_H
