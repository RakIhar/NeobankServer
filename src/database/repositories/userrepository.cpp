#include "userrepository.h"

#include <QSqlQuery>
#include <QSqlError>

UserRepository::UserRepository(QSqlDatabase db)
    : m_db(db.isValid() ? db : QSqlDatabase::database())
{
}

User UserRepository::mapUser(const QSqlQuery &query)
{
    User user;
    user.id = query.value("id").toLongLong();
    user.username = query.value("username").toString();
    user.passwordHash = query.value("password_hash").toString();
    user.email = query.value("email").toString();
    user.phone = query.value("phone").toString();
    user.status = query.value("status").toString();
    user.created_at = query.value("created_at").toDateTime();
    user.updated_at = query.value("updated_at").toDateTime();
    return user;
}

std::optional<User> UserRepository::create(const User &user)
{
    QSqlQuery q(m_db);
    q.prepare("INSERT INTO users (username, password_hash, email, phone, status) "
              "VALUES (:username, :password_hash, :email, :phone, :status) "
              "RETURNING id, username, password_hash, email, phone, status, created_at, updated_at");
    q.bindValue(":username", user.username);
    q.bindValue(":password_hash", user.passwordHash);
    q.bindValue(":email", user.email);
    q.bindValue(":phone", user.phone);
    q.bindValue(":status", user.status);

    if (!q.exec())
    {
        qWarning() << "UserRepository::create error" << q.lastError();
        return std::nullopt;
    }
    if (!q.next())
        return std::nullopt;
    return mapUser(q);
}

std::optional<User> UserRepository::findById(qint64 id) const
{
    QSqlQuery q(m_db);
    q.prepare("SELECT id, username, password_hash, email, phone, status, created_at, updated_at "
              "FROM users WHERE id = :id");
    q.bindValue(":id", QVariant::fromValue(id));
    if (!q.exec() || !q.next())
        return std::nullopt;
    return mapUser(q);
}

std::optional<User> UserRepository::findByUsername(const QString &username) const
{
    QSqlQuery q(m_db);
    q.prepare("SELECT id, username, password_hash, email, phone, status, created_at, updated_at "
              "FROM users WHERE username = :username");
    q.bindValue(":username", username);
    if (!q.exec() || !q.next())
        return std::nullopt;
    return mapUser(q);
}

bool UserRepository::updatePassword(qint64 id, const QString &passwordHash)
{
    QSqlQuery q(m_db);
    q.prepare("UPDATE users SET password_hash = :password_hash, updated_at = NOW() WHERE id = :id");
    q.bindValue(":password_hash", passwordHash);
    q.bindValue(":id", QVariant::fromValue(id));
    if (!q.exec())
    {
        qWarning() << "UserRepository::updatePassword error" << q.lastError();
        return false;
    }
    return q.numRowsAffected() > 0;
}

bool UserRepository::updateStatus(qint64 id, const QString &status)
{
    QSqlQuery q(m_db);
    q.prepare("UPDATE users SET status = :status, updated_at = NOW() WHERE id = :id");
    q.bindValue(":status", status);
    q.bindValue(":id", QVariant::fromValue(id));
    if (!q.exec())
    {
        qWarning() << "UserRepository::updateStatus error" << q.lastError();
        return false;
    }
    return q.numRowsAffected() > 0;
}
