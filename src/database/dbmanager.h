#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QJsonObject>
#include <QDateTime>
#include <QFileInfo>
#include <QStringList>
#include "../service/iservice.h"

namespace Database
{

class DataBaseManager : public IService
{
public:
    explicit DataBaseManager();

    QSqlDatabase database() const { return m_postgresql; }
    bool isConnected() const { return m_postgresql.isValid() && m_postgresql.isOpen(); }
    QString lastError() const { return m_lastError; }

private:
    struct DbConfig
    {
        QString host = "localhost";
        quint16 port = 5432;
        QString dbName = "neobank";
        QString user = "app_user";
        QString password;
        QString sslMode;
    };

    QSqlDatabase m_postgresql;
    DbConfig m_config;
    QString m_lastError;

    DataBaseManager(const DataBaseManager&) = delete;
    DataBaseManager& operator=(const DataBaseManager&) = delete;

    bool loadConfig();
    bool openConnection();
    bool ensureSchema();
    bool execStatement(const QString &sql);

    QString configPath() const;
};

}

#endif // DBMANAGER_H

/*
                                         List of relations
 Schema |     Name      | Type  |  Owner   | Persistence | Access method |    Size    | Description
--------+---------------+-------+----------+-------------+---------------+------------+-------------
 public | accounts      | table | postgres | permanent   | heap          | 0 bytes    |
 public | audit_log     | table | postgres | permanent   | heap          | 8192 bytes |
 public | auth_sessions | table | postgres | permanent   | heap          | 8192 bytes |
 public | transactions  | table | postgres | permanent   | heap          | 8192 bytes |
 public | users         | table | postgres | permanent   | heap          | 8192 bytes |
*/
