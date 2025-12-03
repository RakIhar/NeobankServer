#ifndef AUDITLOGREPOSITORY_H
#define AUDITLOGREPOSITORY_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QList>

#include "../models/auditlogentry.h"

class AuditLogRepository
{
public:
    explicit AuditLogRepository(QSqlDatabase db = QSqlDatabase());

    bool append(const AuditLogEntry &entry);
    QList<AuditLogEntry> findByUser(qint64 userId, int limit = 50) const;
    QList<AuditLogEntry> findBySession(const QUuid &sessionId, int limit = 50) const;

private:
    QSqlDatabase m_db;
    static AuditLogEntry mapEntry(const QSqlQuery &query);
};

#endif // AUDITLOGREPOSITORY_H
