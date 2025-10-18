#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QObject>
#include "clientsession.h"
#include <QPointer>
#include <QSet>
#include <QTcpSocket>
#include <QSslSocket>
#include <QPointer>

class SessionManager : public QObject
{
    Q_OBJECT
public:
    explicit SessionManager(QObject *parent = nullptr);

    ClientSession* createUnauthenticatedSession(QSslSocket* socket);
    // ClientSession* findBySessionId(const QByteArray &sessionId) const;
private:
    QHash<QByteArray, QPointer<ClientSession>> m_authenticatedSessions;
private slots:
    void onSessionClosed(ClientSession *session);
    void onSessionAuthenticated(QByteArray sessionID, ClientSession *session);
    //потом добавить очистку закрытых сессий
signals:
};

#endif // SESSIONMANAGER_H
