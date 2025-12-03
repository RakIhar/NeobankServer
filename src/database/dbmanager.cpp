#include "dbmanager.h"

#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

namespace {
    constexpr auto CONNECTION_NAME = "neobank_pg_conn";
}

DataBaseManager *DataBaseManager::instance()
{
    static DataBaseManager inst;
    return &inst;
}

DataBaseManager::DataBaseManager(QObject *parent)
    : QObject{parent}
{
    if (!loadConfig())
        qCritical() << "Не удалось загрузить конфигурацию БД:" << m_lastError;
    else if (!openConnection())
        qCritical() << "Ошибка подключения к БД:" << m_lastError;
}

bool DataBaseManager::loadConfig()
{
    QFile file(configPath());
    if(!file.open(QIODevice::OpenModeFlag::ReadOnly))
    {
        m_lastError = QStringLiteral("Не удалось открыть файл конфигурации %1")
                          .arg(file.fileName());
        return false;
    }

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject())
    {
        m_lastError = QStringLiteral("Некорректный JSON в %1").arg(file.fileName());
        return false;
    }

    const QJsonObject obj = doc.object();
    m_config.host = obj.value("host").toString(m_config.host);
    m_config.port = static_cast<quint16>(obj.value("port").toInt(m_config.port));
    m_config.dbName = obj.value("database").toString(m_config.dbName);
    m_config.user = obj.value("user").toString(m_config.user);
    m_config.password = qEnvironmentVariable("PG_PASSWORD",
                                             obj.value("password").toString().toUtf8().constData());
    m_config.sslMode = obj.value("sslMode").toString();

    return true;
}

bool DataBaseManager::openConnection()
{
    if (QSqlDatabase::contains(CONNECTION_NAME))
        m_postgresql = QSqlDatabase::database(CONNECTION_NAME);
    else
        m_postgresql = QSqlDatabase::addDatabase("QPSQL", CONNECTION_NAME);

    m_postgresql.setHostName(m_config.host);
    m_postgresql.setPort(static_cast<int>(m_config.port));
    m_postgresql.setDatabaseName(m_config.dbName);
    m_postgresql.setUserName(m_config.user);
    m_postgresql.setPassword(m_config.password);

    if (!m_postgresql.open())
    {
        m_lastError = m_postgresql.lastError().text();
        return false;
    }

    qInfo() << "Подключено к базе данных" << m_config.dbName
            << "как" << m_config.user << "на" << m_config.host << ":" << m_config.port;
    return true;
}

bool DataBaseManager::execStatement(const QString &sql)
{
    QSqlQuery query(m_postgresql);
    if (!query.exec(sql))
    {
        m_lastError = query.lastError().text();
        qCritical() << "Ошибка выполнения SQL:" << sql << m_lastError;
        return false;
    }
    return true;
}

QString DataBaseManager::configPath() const
{
    const QString baseDir = QCoreApplication::applicationDirPath();
    QFileInfo configFile(baseDir + "/../../config/database.json");
    if (configFile.exists())
        return configFile.absoluteFilePath();
    return QStringLiteral("config/database.json");
}
