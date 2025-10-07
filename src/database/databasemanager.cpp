#include "databasemanager.h"

DataBaseManager::DataBaseManager(QObject *parent)
    : QObject{parent}
{}


/*
#include "database.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QCoreApplication>

#include <QPluginLoader>

void DataBase::addDataBase()
{
    qDebug() << QSqlDatabase::drivers();
    qDebug() << QCoreApplication::libraryPaths();
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("mydb");
    db.setUserName("postgres");
    db.setPassword("Haipor123");

    if (!db.open()) {
        qDebug() << "Ошибка подключения:" << db.lastError().text();
    }
}
*/
