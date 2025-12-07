#ifndef TRANSACTIONREPOSITORY_H
#define TRANSACTIONREPOSITORY_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "../models/transaction.h"
#include "../../service/iservice.h"

#include "../dbmanager.h"

namespace Database
{

class TransactionRepository : public IService
{
public:
    explicit TransactionRepository(QSqlDatabase db = QSqlDatabase()) : m_db(db){}
    explicit TransactionRepository(DataBaseManager* dbm) : m_db(dbm->database())
        { qDebug() << "ctor TransactionRepository"; };
    std::optional<Models::Transaction> addTransaction(const Models::Transaction &t);
    QList<Models::Transaction> getByAccount(qint64 accountId) const;
    QList<Models::Transaction> getRecentForUser(qint64 userId, int limit = 20) const;

private:
    QSqlDatabase m_db;

    static Models::Transaction mapTransaction(const QSqlQuery &query);
};

}

#endif // TRANSACTIONREPOSITORY_H
