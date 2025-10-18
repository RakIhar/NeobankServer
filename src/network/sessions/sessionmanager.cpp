#include "sessionmanager.h"

SessionManager::SessionManager(QObject *parent)
    : QObject{parent}
{

}

void SessionManager::createSession(QPointer<QSslSocket> socket)
{
    ClientSession* session = new ClientSession(this, socket);
    connect(session, &ClientSession::expired,
            this, &SessionManager::onSessionExpired);
    m_sessions.insert(session);
}

// void SessionManager::removeSession(ClientSession *session)
// {

// }

// int SessionManager::activeCount() const
// {
//     return m_sessions.count(); //улучшить
// }

void SessionManager::onSessionExpired(ClientSession *session)
{
    m_sessions.remove(session); //улучшить
    session->deleteLater();
}

void SessionManager::cleanup()
{
    auto it = m_sessions.begin();
    while(it != m_sessions.end())
    {
        if (*it == nullptr)
        {
            m_sessions.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

