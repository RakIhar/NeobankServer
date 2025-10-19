#include "sessionmanager.h"

SessionManager::SessionManager(QObject *parent) : QObject(parent){}

ClientSession *SessionManager::createUnauthenticatedSession(QSslSocket *socket)
{
    auto* session = new ClientSession(socket, m_authManager, this);

    connect(session, &ClientSession::closed,
            this, &SessionManager::onSessionClosed);
    connect(session, &ClientSession::authenticated,
            this, &SessionManager::onSessionAuthenticated);

    qDebug() << "createUnauthenticatedSession m_authenticatedSessions: " << m_authenticatedSessions.count(); //DEBUG

    return session;
}

ClientSession *SessionManager::findBySessionId(const QByteArray &sessionId) const
{
    return m_authenticatedSessions.find(sessionId).value();
}

void SessionManager::onSessionClosed(ClientSession *session) //переделать
{
    auto it = std::find_if(m_authenticatedSessions.begin(), m_authenticatedSessions.end(),
                           [session](auto v){ return v == session; });
    if (it != m_authenticatedSessions.end())
        m_authenticatedSessions.erase(it);
    session->deleteLater();
}

void SessionManager::onSessionAuthenticated(QByteArray sessionID, ClientSession *session)
{
    m_authenticatedSessions[sessionID] = session;
}
