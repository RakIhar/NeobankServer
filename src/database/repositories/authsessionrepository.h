#ifndef AUTHSESSIONREPOSITORY_H
#define AUTHSESSIONREPOSITORY_H

#include <optional>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#include "../models/authsession.h"
#include "../../service/iservice.h"

#include "../dbmanager.h"

namespace Database
{

class AuthSessionRepository : public IService
{
public:
    explicit AuthSessionRepository(QSqlDatabase db = QSqlDatabase())
        : m_db(db.isValid() ? db : QSqlDatabase::database()) {}
    explicit AuthSessionRepository(DataBaseManager* dbm) : m_db(dbm->database())
        { qDebug() << "ctor AuthSessionRepository"; };
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

}

#endif // AUTHSESSIONREPOSITORY_H
