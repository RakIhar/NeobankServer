#ifndef AUTHSESSIONREPOSITORY_H
#define AUTHSESSIONREPOSITORY_H

#include <optional>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#include "../models/authsession.h"

class AuthSessionRepository
{
public:
    explicit AuthSessionRepository(QSqlDatabase db = QSqlDatabase());

    std::optional<AuthSession> upsert(const AuthSession &session);
    std::optional<AuthSession> findById(const QUuid &id) const;
    std::optional<AuthSession> findByToken(const QString &token) const;
    bool updateState(const QUuid &id, AuthSessionState state);
    bool deleteSession(const QUuid &id);

private:
    QSqlDatabase m_db;
    static AuthSession mapSession(const QSqlQuery &query);
    static QString toStateString(AuthSessionState state);
    static AuthSessionState fromStateString(const QString &state);
};


#endif // AUTHSESSIONREPOSITORY_H
