#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QObject>
#include <QSslSocket>
#include <QTimer>
#include "socketwrapper.h"

class ConnectionManager : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionManager(QObject *parent = nullptr);
    void CreateConnection(QSslSocket* socket);
    void sendMessage(const QUuid &connection, const QByteArray &rawData);
    void close(const QUuid &connection);
signals:
    void messageReceived(const QUuid &connection, const QByteArray &rawData);
    void closed(const QUuid &connection);
private:
    QHash<QUuid, QPointer<SocketWrapper>> m_socketConnections;
    QTimer m_cleanupTimer;
    void cleanup();
signals:
};

#endif // SESSIONMANAGER_H
