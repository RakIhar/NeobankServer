#ifndef TRANSACTION_SERVICE_H
#define TRANSACTION_SERVICE_H

#include "iservice.h"
#include "comission.h"
#include "currencyexchange.h"
#include "../database/repositories/transactionrepository.h"
#include "../database/repositories/userrepository.h"
#include "../database/repositories/accountrepository.h"

namespace Services
{
using namespace std;

class TransactionService : public IService
{
    Comission* m_com;
    CurrencyExchange* m_curEx;
    Database::UserRepository* m_users;
    Database::TransactionRepository* m_txs;
    Database::AccountRepository* m_accs;

    struct TransferValidationResult {
        Models::Account fromAcc;
        Models::Account toAcc;
        qint64 transferCents;
        qint64 commissionCents;
        qint64 creditCents;
        double rate;
        qint64 fromBal;
        qint64 toBal;
    };
    optional<TransferValidationResult>validateAndCalculate(const QVariant &fromVar, const QVariant &toVar, const QString &amount, QString &errorOut);
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

    optional<Models::Transaction> createTransfer(const QVariant &fromVar,
                                                      const QVariant &toVar,
                                                      const QString &amount,
                                                      const QString &description,
                                                      QString &errorOut);
    optional<pair<Models::Account, Models::Account>> checkFromToAccessibility(
                                                     const QVariant &fromVar,
                                                     const QVariant &toVar,
                                                     QString &errorOut);

    optional<Cents> checkMoneyAccessibility(const QString &fromBal,
                                                   const QString &toBal,
                                                   const QString &trasfer,
                                                   QString &errorOut);
    optional<double>  checkExchangeRateAccessibility(const QString &fromCurrency,
                                                           const QString &toCurrency,
                                                           QString &errorOut);
};

}

#endif // TRANSACTIONSERVICE_H
