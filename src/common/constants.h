#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QAbstractSocket>

enum class Role
{
    None        = 0,  // без прав
    Client      = 1,  // пользователь банка
    Employee    = 2,  // рядовой сотрудник
    Manager     = 3,  // руководитель отдела
    Auditor     = 4,  // только просмотр
    Compliance  = 5,  // AML, KYC, инциденты
    Support     = 6,  // операции по запросу клиента
    Security    = 7,  // мониторинг событий, блокировки
    Admin       = 8,  // административный доступ
    System      = 9   // системные процессы, сервисные аккаунты
};

enum class AuthMethod
{  
    None                     = 0,
    LoginPasswordSms         = 1,
    LoginPasswordEmail       = 2,
    // LoginPasswordScratchCard = 3,
    // LoginPasswordToken       = 4, // Google Authentificator, RSA SecurID
    // Certificate              = 5,
    // Biometrics               = 6,
    // AccessKey                = 7,
    // Token                    = 8,
};

enum class Permission
{
    None        = 0,
    ReadOnly    = 1 << 0,
    NoTransfer  = 1 << 1,
    NoWithdraw  = 1 << 2,
    Frozen      = 1 << 3,
    Blocked     = 1 << 4,
    Closed      = 1 << 5
};

enum class ProtocolType
{

};

enum class JsonField
{
    UserId,
    Username,
    PasswordHash,
    Email,
    AccountId,
    Balance,
    Currency,
    TransactionId,
    Amount,
    Type,
    Timestamp
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


#endif // CONSTANTS_H
