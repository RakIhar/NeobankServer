#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include <optional>
#include <QSqlDatabase>

#include "../models/user.h"
#include "../../service/iservice.h"

#include "../dbmanager.h"

namespace Database
{

class UserRepository : public IService
{
public:
    explicit UserRepository(QSqlDatabase db = QSqlDatabase())
        : m_db(db.isValid() ? db : QSqlDatabase::database()) {}
    explicit UserRepository(DataBaseManager* dbm) : m_db(dbm->database())
        { qDebug() << "ctor UserRepository"; };
    std::optional<Models::User> create(const Models::User &user);
    std::optional<Models::User> findById(qint64 id) const;
    std::optional<Models::User> findByUsername(const QString &username) const;
    bool updatePassword(qint64 id, const QString &passwordHash);
    bool updateStatus(qint64 id, const QString &status);

private:
    QSqlDatabase m_db;
    static Models::User mapUser(const QSqlQuery &query);
};

}

#endif // USERREPOSITORY_H
