#include "sessionmanager.h"
#include <QPointer>

ConnectionManager::ConnectionManager(QObject *parent)
    : QObject{parent}
{
    constexpr int HOUSEKEEPING_INTERVAL_MS = 60000; // 1 минута
    m_cleanupTimer.setInterval(HOUSEKEEPING_INTERVAL_MS);
    m_cleanupTimer.setSingleShot(false);
    connect(&m_cleanupTimer, &QTimer::timeout,
            this, &ConnectionManager::cleanup);
    m_cleanupTimer.start();
}

void ConnectionManager::CreateConnection(QSslSocket *socket)
{
    if (!socket)
        return;

    const QUuid sessionId = QUuid::createUuid();
    SocketWrapper* session = new SocketWrapper(sessionId, socket, this);

    connect(session, &SocketWrapper::closed, this, [this](const QUuid &sessionId){
        auto it = m_socketConnections.find(sessionId);
        if (it == m_socketConnections.end())
            return;

        if (!it.value().isNull())
            it.value()->deleteLater();

        m_socketConnections.erase(it); //CHECK
        emit closed(sessionId);
    });

    connect(session, &SocketWrapper::messageReceived, this, [this](const QUuid &sessionId, const QByteArray &msg){
        emit messageReceived(sessionId, msg);
    });
    m_socketConnections.insert(sessionId, session);
}

void ConnectionManager::sendMessage(const QUuid connection, const QByteArray rawData)
{
    if (m_socketConnections.contains(connection))
        m_socketConnections.value(connection)->sendData(rawData);
}

void ConnectionManager::close(const QUuid connection)
{
    if (m_socketConnections.contains(connection))
        m_socketConnections.value(connection)->close();
}

void ConnectionManager::cleanup()
{
    auto it = m_socketConnections.begin();
    while (it != m_socketConnections.end())
    {
        if (it.value().isNull())
            it = m_socketConnections.erase(it);
        else
            ++it;
    }
}
