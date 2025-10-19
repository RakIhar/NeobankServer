#ifndef TRANSACTIONREPOSITORY_H
#define TRANSACTIONREPOSITORY_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "../models/transaction.h"

class TransactionRepository
{
public:
    explicit TransactionRepository(QSqlDatabase db);

    bool addTransaction(const Transaction &t);
    QList<Transaction> getByAccount(int accountId);

private:
    QSqlDatabase m_db;
};

#endif // TRANSACTIONREPOSITORY_H
