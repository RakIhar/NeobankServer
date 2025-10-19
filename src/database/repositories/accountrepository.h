#ifndef ACCOUNTREPOSITORY_H
#define ACCOUNTREPOSITORY_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "../models/account.h"

class AccountRepository
{
public:
    explicit AccountRepository(QSqlDatabase db);

    bool create(const Account &account);
    Account getById(int id);
    bool updateBalance(int id, double newBalance);

private:
    QSqlDatabase m_db;
};

#endif // ACCOUNTREPOSITORY_H
