#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H
#include "socketsession.h"
#include "../../database/dbmanager.h"

#include <QObject>
#include <QPointer>
#include <QSet>
#include <QTcpSocket>
#include <QSslSocket>
#include <QPointer>

//Единая точка управления сессиями:
//Поиск, фильтрация, оповещение, закрытие и прочее - потом реализовать
//На данный момент класс реализован

class SessionManager : public QObject
{
    Q_OBJECT
public:
    static SessionManager* instance();
    SocketSession* CreateSocketSession(QSslSocket* socket);
private:
    QHash<QByteArray, QPointer<SocketSession>> m_socketSessions;
private:
    explicit SessionManager(QObject *parent = nullptr);
    SessionManager(const SessionManager&) = delete;
    SessionManager& operator=(const SessionManager&) = delete;

private slots:
    void onSessionClosed(SocketSession *session);
signals:
};

#endif // SESSIONMANAGER_H
