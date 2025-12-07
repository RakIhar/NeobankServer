#ifndef AUDITLOGREPOSITORY_H
#define AUDITLOGREPOSITORY_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QList>

#include "../models/auditlogentry.h"
#include "../../service/iservice.h"

#include "../dbmanager.h"

namespace Database
{

class AuditLogRepository : public IService
{
public:
    explicit AuditLogRepository(QSqlDatabase db = QSqlDatabase())
        : m_db(db.isValid() ? db : QSqlDatabase::database()) {}
    explicit AuditLogRepository(DataBaseManager* dbm) : m_db(dbm->database())
        { qDebug() << "ctor AuditLogRepository"; };
    bool append(const Models::AuditLogEntry &entry);
    QList<Models::AuditLogEntry> findByUser(qint64 userId, int limit = 50) const;
    QList<Models::AuditLogEntry> findBySession(const QUuid &sessionId, int limit = 50) const;

private:
    QSqlDatabase m_db;
    static Models::AuditLogEntry mapEntry(const QSqlQuery &query);
};

}

#endif // AUDITLOGREPOSITORY_H
