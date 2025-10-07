#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>

class DataBaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DataBaseManager(QObject *parent = nullptr);

signals:
};

#endif // DBMANAGER_H
