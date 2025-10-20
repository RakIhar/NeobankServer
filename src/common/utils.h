#ifndef UTILS_H
#define UTILS_H
#include <QString>
#include "constants.h"

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

#endif // UTILS_H
