#ifndef TRANSACTIONREPOSITORY_H
#define TRANSACTIONREPOSITORY_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "../models/transaction.h"

class TransactionRepository
{
public:
    explicit TransactionRepository(QSqlDatabase db = QSqlDatabase());

    std::optional<Transaction> addTransaction(const Transaction &t);
    QList<Transaction> getByAccount(qint64 accountId) const;
    QList<Transaction> getRecentForUser(qint64 userId, int limit = 20) const;

private:
    QSqlDatabase m_db;

    static Transaction mapTransaction(const QSqlQuery &query);
};

#endif // TRANSACTIONREPOSITORY_H
