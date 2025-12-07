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
    std::optional<Models::AuthSession> upsert(const Models::AuthSession &session);
    std::optional<Models::AuthSession> findById(const QUuid &id) const;
    std::optional<Models::AuthSession> findByToken(const QString &token) const;
    bool updateState(const QUuid &id, Models::AuthSessionState state);
    bool deleteSession(const QUuid &id);

private:
    QSqlDatabase m_db;
    static Models::AuthSession mapSession(const QSqlQuery &query);
    static QString toStateString(Models::AuthSessionState state);
    static Models::AuthSessionState fromStateString(const QString &state);
};

}

#endif // AUTHSESSIONREPOSITORY_H
