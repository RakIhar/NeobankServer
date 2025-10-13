#include "dbmanager.h"

DataBaseManager::DataBaseManager(QObject *parent)
    : QObject{parent}
{

    m_postgresql = QSqlDatabase::addDatabase("QPSQL");
    m_postgresql.setHostName("localhost");
    m_postgresql.setPort(5432);
    m_postgresql.setDatabaseName("NeoBank");
    m_postgresql.setUserName("postgres");
    auto password = qgetenv("DB_NEOBANK_PASSWORD");
    m_postgresql.setPassword(password);
    if (!m_postgresql.open())
        qDebug() << m_postgresql.lastError().text();
    else
        qDebug() << "Успешно подключено";

}
