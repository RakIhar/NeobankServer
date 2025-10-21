#ifndef UTILS_H
#define UTILS_H
#include <QString>
#include "constants.h"
#include <QAbstractSocket>

class Utils
{
public:
    Utils();
};

inline QString toString(JsonField field)
{
    switch(field)
    {
    case JsonField::UserId: return "user_id";
    case JsonField::Username: return "username";
    case JsonField::PasswordHash: return "password_hash";
    case JsonField::Email: return "email";
    case JsonField::AccountId: return "account_id";
    case JsonField::Balance: return "balance";
    case JsonField::Currency: return "currency";
    case JsonField::TransactionId: return "transaction_id";
    case JsonField::Amount: return "amount";
    case JsonField::Type: return "type";
    case JsonField::Timestamp: return "timestamp";
    }
    return {};
}

inline QString toString(QAbstractSocket::SocketState st)
{
    switch(st) {
    case QAbstractSocket::UnconnectedState: return "Не подключен";
    case QAbstractSocket::HostLookupState: return "Идет поиск хоста";
    case QAbstractSocket::ConnectingState: return "Подключение";
    case QAbstractSocket::ConnectedState: return "Подключен"; //Для клиента основное состояние
    case QAbstractSocket::ClosingState: return "Закрывается";
    case QAbstractSocket::BoundState: return "Открыт на сервере и слушает входящие соединения"; //не для клиента
    case QAbstractSocket::ListeningState: return "Привязан к локальному адресу и порту"; //не для клиента
    }
    return {};
}

#endif // UTILS_H
