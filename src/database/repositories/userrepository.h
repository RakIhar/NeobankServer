#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include <optional>
#include <QSqlDatabase>

#include "../models/user.h"

class UserRepository
{
public:
    explicit UserRepository(QSqlDatabase db = QSqlDatabase());

    std::optional<User> create(const User &user);
    std::optional<User> findById(qint64 id) const;
    std::optional<User> findByUsername(const QString &username) const;
    bool updatePassword(qint64 id, const QString &passwordHash);
    bool updateStatus(qint64 id, const QString &status);

private:
    QSqlDatabase m_db;
    static User mapUser(const QSqlQuery &query);
};

#endif // USERREPOSITORY_H
