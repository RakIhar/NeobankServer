#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H
#include "clientsession.h"
#include "../../database/dbmanager.h"

#include <QObject>
#include <QPointer>
#include <QSet>
#include <QTcpSocket>
#include <QSslSocket>
#include <QPointer>

//Создаёт, удаляет и хранит сессии

class SessionManager : public QObject
{
    Q_OBJECT
public:
    explicit SessionManager(QObject *parent = nullptr);

    ClientSession* createUnauthenticatedSession(QSslSocket* socket);
    ClientSession* findBySessionId(const QByteArray &sessionId) const;
private:
    QHash<QByteArray, QPointer<ClientSession>> m_authenticatedSessions;
    AuthManager* m_authManager;
    DataBaseManager* m_dbManager;
    //TODO:
    //убрать сигнал аутентификации:
    //хранить просто сессии + методы для фильтрации и методы оповещение
private slots:
    void onSessionClosed(ClientSession *session);
    void onSessionAuthenticated(QByteArray sessionID, ClientSession *session);
signals:
};

#endif // SESSIONMANAGER_H
