#include "sessionmanager.h"

SessionManager::SessionManager(QObject *parent)
    : QObject{parent}
{

}

void SessionManager::createSession(QPointer<QSslSocket> socket)
{
    ClientSession* session = new ClientSession(this, socket);
    connect(session, &ClientSession::expired ,this, &SessionManager::onSessionExpired);
    m_sessions.insert(session);
}

void SessionManager::onSessionExpired(ClientSession *session)
{
    m_sessions.remove(session);
    session->deleteLater();
}


// connect(sslSocket, &QSslSocket::disconnected, this, [this, sslSocket]() {
    // m_activeSockets.remove(sslSocket);
    // sslSocket->deleteLater();
// });
