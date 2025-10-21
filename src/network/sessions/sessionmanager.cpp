#include "sessionmanager.h"

SessionManager::SessionManager(QObject *parent) : QObject(parent) {}

SessionManager *SessionManager::instance()
{
    static SessionManager inst;
    return &inst;
}

SocketSession *SessionManager::CreateSocketSession(QSslSocket *socket)
{
    auto* session = new SocketSession(socket, this);
    connect(session, &SocketSession::closed,
            this, &SessionManager::onSessionClosed);
    return session;
}

void SessionManager::onSessionClosed(SocketSession *session)
{
    qDebug() << "SessionManager::onSessionClosed";
    auto it = std::find_if(m_socketSessions.begin(), m_socketSessions.end(),
                           [session](auto v){ return v == session; });
    if (it != m_socketSessions.end())
        m_socketSessions.erase(it);
    session->deleteLater();
}
