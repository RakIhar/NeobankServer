#include "authsessionrepository.h"

#include <QVariant>
using namespace Database;
    namespace {
    QString uuidToString(const QUuid &id)
    {
        return id.toString(QUuid::WithoutBraces);
    }
}

QString AuthSessionRepository::toStateString(Models::AuthSessionState state)
{
    switch (state) {
    case Models::AuthSessionState::Pending: return QStringLiteral("pending");
    case Models::AuthSessionState::Established: return QStringLiteral("established");
    case Models::AuthSessionState::Expired: return QStringLiteral("expired");
    case Models::AuthSessionState::Revoked: return QStringLiteral("revoked");
    }
    return QStringLiteral("pending");
}

Models::AuthSessionState AuthSessionRepository::fromStateString(const QString &state)
{
    const QString normalized = state.toLower();
    if (normalized == "established")
        return Models::AuthSessionState::Established;
    if (normalized == "expired")
        return Models::AuthSessionState::Expired;
    if (normalized == "revoked")
        return Models::AuthSessionState::Revoked;
    return Models::AuthSessionState::Pending;
}

Models::AuthSession AuthSessionRepository::mapSession(const QSqlQuery &query)
{
    Models::AuthSession session;
    session.id = QUuid(query.value("id").toString());
    session.user_id = query.value("user_id").toLongLong();
    session.state = fromStateString(query.value("state").toString());
    session.token = query.value("token").toString();
    session.ip_address = query.value("ip_address").toString();
    session.user_agent = query.value("user_agent").toString();
    session.created_at = query.value("created_at").toDateTime();
    session.expires_at = query.value("expires_at").toDateTime();
    return session;
}

std::optional<Models::AuthSession> AuthSessionRepository::upsert(const Models::AuthSession &session)
{
    QSqlQuery q(m_db);
    q.prepare("INSERT INTO auth_sessions "
              "(id, user_id, state, token, ip_address, user_agent, created_at, expires_at) "
              "VALUES (:id, :user_id, :state, :token, :ip_address, :user_agent, :created_at, :expires_at) "
              "ON CONFLICT (id) DO UPDATE SET "
              "user_id = EXCLUDED.user_id,"
              "state = EXCLUDED.state,"
              "token = EXCLUDED.token,"
              "ip_address = EXCLUDED.ip_address,"
              "user_agent = EXCLUDED.user_agent,"
              "created_at = LEAST(auth_sessions.created_at, EXCLUDED.created_at),"
              "expires_at = EXCLUDED.expires_at "
              "RETURNING id, user_id, state, token, ip_address, user_agent, created_at, expires_at");
    q.bindValue(":id", uuidToString(session.id));
    q.bindValue(":user_id", session.user_id);
    q.bindValue(":state", toStateString(session.state));
    q.bindValue(":token", session.token);
    q.bindValue(":ip_address", session.ip_address);
    q.bindValue(":user_agent", session.user_agent);
    q.bindValue(":created_at", session.created_at);
    q.bindValue(":expires_at", session.expires_at);

    if (!q.exec())
    {
        qWarning() << "AuthSessionRepository::upsert error" << q.lastError();
        return std::nullopt;
    }
    if (!q.next())
        return std::nullopt;
    return mapSession(q);
}

std::optional<Models::AuthSession> AuthSessionRepository::findById(const QUuid &id) const
{
    QSqlQuery q(m_db);
    q.prepare("SELECT id, user_id, state, token, ip_address, user_agent, created_at, expires_at "
              "FROM auth_sessions WHERE id = :id");
    q.bindValue(":id", uuidToString(id));
    if (!q.exec() || !q.next())
        return std::nullopt;
    return mapSession(q);
}

std::optional<Models::AuthSession> AuthSessionRepository::findByToken(const QString &token) const
{
    QSqlQuery q(m_db);
    q.prepare("SELECT id, user_id, state, token, ip_address, user_agent, created_at, expires_at "
              "FROM auth_sessions WHERE token = :token");
    q.bindValue(":token", token);
    if (!q.exec() || !q.next())
        return std::nullopt;
    return mapSession(q);
}

bool AuthSessionRepository::updateState(const QUuid &id, Models::AuthSessionState state)
{
    QSqlQuery q(m_db);
    q.prepare("UPDATE auth_sessions SET state = :state WHERE id = :id");
    q.bindValue(":state", toStateString(state));
    q.bindValue(":id", uuidToString(id));
    if (!q.exec())
    {
        qWarning() << "AuthSessionRepository::updateState error" << q.lastError();
        return false;
    }
    return q.numRowsAffected() > 0;
}

bool AuthSessionRepository::deleteSession(const QUuid &id)
{
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM auth_sessions WHERE id = :id");
    q.bindValue(":id", uuidToString(id));
    if (!q.exec())
    {
        qWarning() << "AuthSessionRepository::deleteSession error" << q.lastError();
        return false;
    }
    return q.numRowsAffected() > 0;
}
