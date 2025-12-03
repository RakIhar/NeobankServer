#ifndef ACCOUNTREPOSITORY_H
#define ACCOUNTREPOSITORY_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "../models/account.h"

class AccountRepository
{
public:
    explicit AccountRepository(QSqlDatabase db = QSqlDatabase());
    std::optional<Account> create(const Account &account);
    std::optional<Account> getById(qint64 id) const;
    QList<Account> getByUser(qint64 userId) const;
    bool updateBalance(qint64 id, QString newBalance);
    bool updateStatus(qint64 id, const QString &status);
private:
    QSqlDatabase m_db;
    static Account mapAccount(const QSqlQuery &query);
};

#endif // ACCOUNTREPOSITORY_H
