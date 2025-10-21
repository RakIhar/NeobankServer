#ifndef SOCKETSESSION_H
#define SOCKETSESSION_H

#include <QObject>
#include <QPointer>
#include <QSet>
#include <QSsl>
#include <QSslSocket>
#include <QTcpSocket>
#include <QTimer>
#include <QDeadlineTimer>

//Управляет циклом жизни сессии
//Обрабатывает сокет
//Вероятно будет ещё уровень между SocketSession и бизнес-логикой

//Сейчас всё реализовано

class SocketSession : public QObject
{
    Q_OBJECT
public:
    explicit SocketSession(QSslSocket* socket, QObject *parent = nullptr);

    static constexpr int SESSION_TIMEOUT_MS = 600000;
    static constexpr int SESSION_MAX_LIFETIME_MS = 3600000;
private:
    bool m_isClosed = false;
    QSslSocket* m_socket;
    QTimer m_timer;
    QTimer m_absTimer;
    QByteArray m_buffer;

    void sendData(const QByteArray &msg);
    void processIncomingMessage(const QByteArray &msg);

    void extendLifetime();
    void closeSession();
private slots:
    void onErrorOccurred(QAbstractSocket::SocketError socketError);
    void onReadyRead();
    void onSocketDisconnected();
    void onSocketDestroyed();
signals:
    void closed(SocketSession *self);
};

#endif // SOCKETSESSION_H
