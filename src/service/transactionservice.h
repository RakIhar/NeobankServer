#ifndef TRANSACTION_SERVICE_H
#define TRANSACTION_SERVICE_H

#include "iservice.h"
#include "comission.h"
#include "currencyexchange.h"
#include "../database/repositories/transactionrepository.h"
#include "../database/repositories/userrepository.h"
#include "../database/repositories/accountrepository.h"

namespace Services {

class TransactionService : public IService
{
    Comission* m_com;
    CurrencyExchange* m_curEx;
    Database::UserRepository* m_users;
    Database::TransactionRepository* m_txs;
    Database::AccountRepository* m_accs;
public:
    struct Cents
    {
        qint64 fromBal;
        qint64 toBal;
        qint64 transfer;
    };

    TransactionService(Comission* com,
                       CurrencyExchange* curEx,
                       Database::UserRepository* users,
                       Database::TransactionRepository* txs,
                       Database::AccountRepository* accs)
        : m_com(com), m_curEx(curEx), m_users(users), m_txs(txs), m_accs(accs) {}

    BeforeTransferInfo getBeforeTransferInfo(const QString& to, const QString &amount, const QString &from);

    std::optional<Models::Transaction> createTransfer(const QVariant &fromVar,
                                                      const QVariant &toVar,
                                                      const QString &amount,
                                                      const QString &description,
                                                      QString &errorOut);
    std::tuple<Models::Account, Models::Account, bool> checkFromToAccessibility(
                                                     const QVariant &fromVar,
                                                     const QVariant &toVar,
                                                     QString &errorOut);

    std::pair<Cents, bool> checkMoneyAccessibility(const QString &fromBal,
                                                   const QString &toBal,
                                                   const QString &trasfer,
                                                   QString &errorOut);
    std::pair<qint64, bool> checkExchangeRateAccessibility(const QString &fromCurrency,
                                                           const QString &toCurrency,
                                                           QString &errorOut);
};

}

#endif // TRANSACTIONSERVICE_H
