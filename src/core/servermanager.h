#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>

class ServerManager : public QObject
{
    Q_OBJECT
public:
    explicit ServerManager(QObject *parent = nullptr);

signals:
};

#endif // SERVERMANAGER_H
