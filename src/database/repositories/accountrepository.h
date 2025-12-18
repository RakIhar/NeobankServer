#ifndef ACCOUNTREPOSITORY_H
#define ACCOUNTREPOSITORY_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "../models/account.h"
#include "../../service/iservice.h"

#include "../dbmanager.h"
namespace Database
{

class AccountRepository : public IService
{
public:
    explicit AccountRepository(QSqlDatabase db = QSqlDatabase())
        : m_db(db.isValid() ? db : QSqlDatabase::database()) {}
    explicit AccountRepository(DataBaseManager* dbm) : m_db(dbm->database())
        { qDebug() << "ctor AccountRepository"; };
    std::optional<Models::Account> create(const Models::Account &account);
    std::optional<Models::Account> getById(qint64 id) const;
    std::optional<Models::Account> getByIban(const QString &iban) const;
    QList<Models::Account> getByUser(qint64 userId) const;
    bool updateBalance(qint64 id, QString newBalance);
    bool updateStatus(qint64 id, const QString &status);
private:
    QSqlDatabase m_db;
    static Models::Account mapAccount(const QSqlQuery &query);
};

}

#endif // ACCOUNTREPOSITORY_H
