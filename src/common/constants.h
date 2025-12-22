#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QAbstractSocket>
#include <QMetaEnum>
#include <qdatetime.h>
#include "../database/models/account.h"

class Enums : public QObject
{
    Q_OBJECT
public:
    enum class Currency
    {
        BYN,
        RUB,
        USD,
        EUR,
        GBP,
        KZT,
        UAH,
        PLN,
        CNY
    };
    Q_ENUM(Currency)

    static inline QString toSymbol(Currency currency)
    {
        switch(currency) {
        case Currency::BYN: return "Br";
        case Currency::RUB: return "₽";
        case Currency::USD: return "$";
        case Currency::EUR: return "€";
        case Currency::GBP: return "£";
        case Currency::CNY: return "¥";
        case Currency::KZT: return "₸";
        default:            return "";
        }
    }

    static inline QString toStr(Currency currency)
    {
        const int value = static_cast<int>(currency);
        const char* key = QMetaEnum::fromType<Currency>().valueToKey(value);
        return key ? QString::fromLatin1(key) : QString();
    }

    static inline std::pair<Currency, bool> fromStr(const QString &str, Currency defaultValue = Currency::BYN ) {
        bool isCorrect;
        int value = QMetaEnum::fromType<Currency>().keyToValue(str.toUpper().toLatin1().data(), &isCorrect);
        return std::make_pair(isCorrect ? static_cast<Currency>(value) : defaultValue, isCorrect);
    }
};

Q_DECLARE_METATYPE(Enums::Currency)

struct ExchangeData
{
    Enums::Currency from;
    Enums::Currency to;
    double exchangeRate;
    QString exchangeRateStr;
    QDateTime dt;//not utc??
};

struct BeforeTransferInfo
{
    QString comission = "";
    QString exchangeRate = "";
    QString resultAmount = "";
    std::optional<Models::Account> to_acc = std::nullopt;
    bool isAllowed;
    QString error;
};

namespace Common  {

// enum class Role
// {
//     None        = 0,  // без прав
//     Client      = 1,  // пользователь банка
//     Employee    = 2,  // рядовой сотрудник
//     Manager     = 3,  // руководитель отдела
//     Auditor     = 4,  // только просмотр
//     Compliance  = 5,  // AML, KYC, инциденты
//     Support     = 6,  // операции по запросу клиента
//     Security    = 7,  // мониторинг событий, блокировки
//     Admin       = 8,  // административный доступ
//     System      = 9   // системные процессы, сервисные аккаунты
// };

// enum class AuthMethod
// {
//     None                     = 0,
//     LoginPasswordSms         = 1,
//     LoginPasswordEmail       = 2,
//     // LoginPasswordScratchCard = 3,
//     // LoginPasswordToken       = 4, // Google Authentificator, RSA SecurID
//     // Certificate              = 5,
//     // Biometrics               = 6,
//     // AccessKey                = 7,
//     // Token                    = 8,
// };

// enum class Permission
// {
//     None        = 0,
//     ReadOnly    = 1 << 0,
//     NoTransfer  = 1 << 1,
//     NoWithdraw  = 1 << 2,
//     Frozen      = 1 << 3,
//     Blocked     = 1 << 4,
//     Closed      = 1 << 5
// };

enum class AccStatus
{
    Active,
    Frozen,
    Deleted,
    System
};

inline QString toStr(AccStatus st)
{
    switch(st)
    {
    case AccStatus::Active:  return "active";
    case AccStatus::Frozen:  return "frozen";
    case AccStatus::Deleted: return "deleted";
    case AccStatus::System:  return "system";
    }
    return {};
}

inline AccStatus fromStr(const QString& statusStr, AccStatus defaultValue = AccStatus::Deleted)
{
    static const QMap<QString, AccStatus> stringToEnum = {
        {"active",  AccStatus::Active},
        {"frozen",  AccStatus::Frozen},
        {"deleted", AccStatus::Deleted},
        {"system",  AccStatus::System}
    };

    return stringToEnum.value(statusStr.toLower(), defaultValue);
}

enum class ProtocolType
{
    Login,
    Register,
    Authorization,
    AccList,
    TrList,
    AccCreate,
    AccDelete,
    TrCreate,
    TrBefore,
    CreditCreate,
    Undefined,
    ExchangeRate
};

inline QString toStr(ProtocolType type)
{
    switch(type) {
    case ProtocolType::Login:         return "login";
    case ProtocolType::Register:      return "register";
    case ProtocolType::Authorization: return "authorization";
    case ProtocolType::AccList:       return "account.list";
    case ProtocolType::TrList:        return "transaction.list";
    case ProtocolType::AccCreate:     return "account.create";
    case ProtocolType::AccDelete:     return "account.delete";
    case ProtocolType::TrCreate:      return "transaction.create";
    case ProtocolType::TrBefore:      return "transaction.before";
    case ProtocolType::CreditCreate:  return "credit.create";
    case ProtocolType::Undefined:     return "undef";
    case ProtocolType::ExchangeRate:  return "exchange_rate";
    }
    return {};
}

enum class JsonField
{
    UserId,
    AccountId,
    Iban,
    Balance,
    Currency,
    Status,
    CreatedAt,
    UpdatedAt,
    CounterpartyId,
    FromAcc,
    ToAcc,
    Username,
    Password,
    PasswordHash,
    Email,
    TransactionId,
    Amount,
    Type,
    Subtype,
    Timestamp,
    SessionId,
    Token,
    Result,
    Error,
    Descr,
    ReasonCode,
    Phone,
    Metadata,
    AccObj,
    AccArr,
    TrObj,
    TrArr,
    Obj, //плохое поле
    Limit,
    Page,
    Comission,
    ExchangeRate,
    Count,
    ResAmount
};

inline QString toStr(JsonField field)
{
    switch(field)
    {
    case JsonField::UserId:         return "user_id";
    case JsonField::Username:       return "username";
    case JsonField::Password:       return "password";
    case JsonField::PasswordHash:   return "password_hash";
    case JsonField::Email:          return "email";
    case JsonField::AccountId:      return "account_id";
    case JsonField::Balance:        return "balance";
    case JsonField::Currency:       return "currency";
    case JsonField::TransactionId:  return "transaction_id";
    case JsonField::Amount:         return "amount";
    case JsonField::ResAmount:      return "res_amount";
    case JsonField::Type:           return "type";
    case JsonField::Subtype:        return "subtype";
    case JsonField::Timestamp:      return "timestamp";
    case JsonField::SessionId:      return "session_id";
    case JsonField::Token:          return "token";
    case JsonField::Result:         return "result";
    case JsonField::Error:          return "error";
    case JsonField::ReasonCode:     return "reason_code";
    case JsonField::Phone:          return "phone";
    case JsonField::Iban:           return "iban";
    case JsonField::Status:         return "status";
    case JsonField::CreatedAt:      return "created_at";
    case JsonField::UpdatedAt:      return "updated_at";
    case JsonField::AccObj:         return "account";
    case JsonField::AccArr:         return "accounts";
    case JsonField::TrObj:          return "transaction";
    case JsonField::TrArr:          return "transactions";
    case JsonField::CounterpartyId: return "counterparty_account_id";
    case JsonField::Limit:          return "limit";
    case JsonField::Page:           return "page";
    case JsonField::FromAcc:        return "from";
    case JsonField::ToAcc:          return "to";
    case JsonField::Metadata:       return "metadata";
    case JsonField::Obj:            return "obj";
    case JsonField::Comission:      return "comission";
    case JsonField::ExchangeRate:   return "exchane_rate";
    case JsonField::Descr:          return "description";
    case JsonField::Count:          return "count";
    }
    return {};
}

inline QString toStr(QAbstractSocket::SocketState st)
{
    switch(st) {
    case QAbstractSocket::UnconnectedState: return "Не подключен";
    case QAbstractSocket::HostLookupState:  return "Идет поиск хоста";
    case QAbstractSocket::ConnectingState:  return "Подключение";
    case QAbstractSocket::ConnectedState:   return "Подключен"; //Для клиента основное состояние
    case QAbstractSocket::ClosingState:     return "Закрывается";
    case QAbstractSocket::BoundState:       return "Открыт на сервере и слушает входящие соединения"; //не для клиента
    case QAbstractSocket::ListeningState:   return "Привязан к локальному адресу и порту"; //не для клиента
    }
    return {};
}

}

#endif // CONSTANTS_H
