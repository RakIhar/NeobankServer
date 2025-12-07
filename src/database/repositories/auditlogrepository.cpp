#include "auditlogrepository.h"

using namespace Database;
#include <QJsonDocument>
#include <QVariant>

namespace {
QString uuidToString(const QUuid &id)
{
    return id.toString(QUuid::WithoutBraces);
}
}



AuditLogEntry AuditLogRepository::mapEntry(const QSqlQuery &query)
{
    AuditLogEntry entry;
    entry.id = query.value("id").toLongLong();
    entry.session_id = QUuid(query.value("session_id").toString());
    entry.user_id = query.value("user_id").toLongLong();
    entry.action = query.value("action").toString();
    const QVariant detailsValue = query.value("details");
    if (!detailsValue.isNull())
    {
        const QJsonDocument doc = QJsonDocument::fromJson(detailsValue.toByteArray());
        if (doc.isObject())
            entry.details = doc.object();
    }
    entry.created_at = query.value("created_at").toDateTime();
    return entry;
}

bool AuditLogRepository::append(const AuditLogEntry &entry)
{
    QSqlQuery q(m_db);
    q.prepare("INSERT INTO audit_log (session_id, user_id, action, details) "
              "VALUES (:session_id, :user_id, :action, :details)");
    q.bindValue(":session_id", uuidToString(entry.session_id));
        q.bindValue(":user_id", entry.user_id);
    q.bindValue(":action", entry.action);
    q.bindValue(":details", QJsonDocument(entry.details).toJson(QJsonDocument::Compact));

    if (!q.exec())
    {
        qWarning() << "AuditLogRepository::append error" << q.lastError();
        return false;
    }
    return true;
}

QList<AuditLogEntry> AuditLogRepository::findByUser(qint64 userId, int limit) const
{
    QList<AuditLogEntry> entries;
    QSqlQuery q(m_db);
    q.prepare("SELECT id, session_id, user_id, action, details, created_at "
              "FROM audit_log WHERE user_id = :user_id "
              "ORDER BY created_at DESC LIMIT :limit");
    q.bindValue(":user_id", QVariant::fromValue(userId));
    q.bindValue(":limit", limit);
    if (!q.exec())
        while (q.next())
            entries.append(mapEntry(q));
    return entries;
}

QList<AuditLogEntry> AuditLogRepository::findBySession(const QUuid &sessionId, int limit) const
{
    QList<AuditLogEntry> entries;
    QSqlQuery q(m_db);
    q.prepare("SELECT id, session_id, user_id, action, details, created_at "
              "FROM audit_log WHERE session_id = :session_id "
              "ORDER BY created_at DESC LIMIT :limit");
    q.bindValue(":session_id", uuidToString(sessionId));
    q.bindValue(":limit", limit);
    if (q.exec())
        while (q.next())
            entries.append(mapEntry(q));
    return entries;
}
