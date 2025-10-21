#include "dbmanager.h"

DataBaseManager *DataBaseManager::instance()
{
    static DataBaseManager inst;
    return &inst;
}

DataBaseManager::DataBaseManager(QObject *parent)
    : QObject{parent}
{
    m_postgresql = QSqlDatabase::addDatabase("QPSQL");
    m_postgresql.setHostName("localhost");
    m_postgresql.setPort(5432);


    m_postgresql.setDatabaseName("neobank");
    m_postgresql.setUserName("app_user");
    m_postgresql.setPassword(qEnvironmentVariable("PG_PASSWORD"));
    if (!m_postgresql.open())
    {
        qDebug() << m_postgresql.lastError().text();
    }
    else
    {
        qDebug() << "Успешно подключено к базе данных";
    }
}
