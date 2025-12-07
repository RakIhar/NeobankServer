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
    std::optional<User> create(const User &user);
    std::optional<User> findById(qint64 id) const;
    std::optional<User> findByUsername(const QString &username) const;
    bool updatePassword(qint64 id, const QString &passwordHash);
    bool updateStatus(qint64 id, const QString &status);

private:
    QSqlDatabase m_db;
    static User mapUser(const QSqlQuery &query);
};

}

#endif // USERREPOSITORY_H
