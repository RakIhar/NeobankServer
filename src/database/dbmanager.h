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
    explicit DataBaseManager(QObject *parent = nullptr);
private:
    QSqlDatabase m_postgresql;
signals:
};

#endif // DBMANAGER_H
