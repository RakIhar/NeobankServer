#ifndef CLIENTSESSION_H
#define CLIENTSESSION_H

#include <QObject>
#include <QPointer>
#include <QSet>
#include <QSsl>
#include <QSslSocket>
#include <QTcpSocket>
#include <QTimer>
#include "../../security/authentification/authmanager.h"

//Управляет циклом жизни сессии
//Обрабатывает сокет
//Вероятно будет ещё уровень между ClientSession и бизнес-логикой

class ClientSession : public QObject
{
    Q_OBJECT
public:
    explicit ClientSession(QSslSocket* socket, AuthManager* authManager, QObject *parent = nullptr);
private:
    QSslSocket* m_socket;
    AuthManager* m_authManager;
    QTimer m_timer;

    void sendData(const QByteArray &data);
    void processIncomingMessage(const QByteArray &data);

    void extendLifetime();

    AuthContext m_authContext;

    bool m_isAuthenticated = false;    
    QByteArray m_buffer;
private slots:
    void onErrorOccurred(QAbstractSocket::SocketError socketError);
    void onReadyRead();
    void onSocketDisconnected();
    void onSocketDestroyed();
signals:
    void closed(ClientSession *self);
    void authenticated(QByteArray sessionID, ClientSession *self);
};

#endif // CLIENTSESSION_H
