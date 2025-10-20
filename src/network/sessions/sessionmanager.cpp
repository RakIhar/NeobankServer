#include "sessionmanager.h"

SessionManager::SessionManager(QObject *parent) : QObject(parent)
{
    m_authManager = new AuthManager(this); //TODO: improve architecture
    m_dbManager = new DataBaseManager(this); //TODO: improve architecture
}

ClientSession *SessionManager::createUnauthenticatedSession(QSslSocket *socket)
{
    auto* session = new ClientSession(socket, m_authManager, this);

    connect(session, &ClientSession::closed,
            this, &SessionManager::onSessionClosed);
    connect(session, &ClientSession::authenticated,
            this, &SessionManager::onSessionAuthenticated); //TODO: improve architecture

    return session;
}

ClientSession *SessionManager::findBySessionId(const QByteArray &sessionId) const
{
    return m_authenticatedSessions.find(sessionId).value(); //TODO: improve
}

void SessionManager::onSessionClosed(ClientSession *session) //TODO: improve
{
    auto it = std::find_if(m_authenticatedSessions.begin(), m_authenticatedSessions.end(),
                           [session](auto v){ return v == session; });
    if (it != m_authenticatedSessions.end())
        m_authenticatedSessions.erase(it);
    session->deleteLater();
}

void SessionManager::onSessionAuthenticated(QByteArray sessionID, ClientSession *session) //TODO: improve
{
    m_authenticatedSessions[sessionID] = session;
}
