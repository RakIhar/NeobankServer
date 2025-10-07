#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>

class DataBaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DataBaseManager(QObject *parent = nullptr);

signals:
};

#endif // DATABASEMANAGER_H
