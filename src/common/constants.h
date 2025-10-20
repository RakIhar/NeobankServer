#ifndef CONSTANTS_H
#define CONSTANTS_H

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

#endif // CONSTANTS_H
