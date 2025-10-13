#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QObject>
#include "clientsession.h"
#include <QPointer>
#include <QSet>
#include <QTcpSocket>
#include <QSslSocket>
#include <QPointer>

//хранит все сессии
//создает и удаляет сессии
//управляет временем их жизни

inline size_t qHash(const QPointer<ClientSession> &ptr, size_t seed = 0) noexcept {
    return qHash(ptr.data(), seed);
}

class SessionManager : public QObject
{
    Q_OBJECT
public:
    explicit SessionManager(QObject *parent = nullptr);
    void createSession(QPointer<QSslSocket> socket);
private:
    QSet<QPointer<ClientSession>> m_sessions;

private slots:
    void onSessionExpired(ClientSession *session);

signals:
};

#endif // SESSIONMANAGER_H
