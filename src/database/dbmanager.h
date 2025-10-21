#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

class DataBaseManager : public QObject
{
    Q_OBJECT
public:
    static DataBaseManager* instance();
private:
    QSqlDatabase m_postgresql;
    explicit DataBaseManager(QObject *parent = nullptr);
    DataBaseManager(const DataBaseManager&) = delete;
    DataBaseManager& operator=(const DataBaseManager&) = delete;
signals:
};

#endif // DBMANAGER_H
