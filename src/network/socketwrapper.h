#ifndef SOCKETWRAPPER_H
#define SOCKETWRAPPER_H

#include <QSslSocket>
#include <QUuid>

struct ConnectionInfo
{
    QDateTime createdAt;
    QDateTime lastActivity;
    QByteArray peerAddress;
    unsigned short peerPort = 0;
    QUuid id;
};

class SocketWrapper : public QObject
{
    Q_OBJECT
public:
    explicit SocketWrapper(const QUuid &sessionId,
                           QSslSocket* socket,
                           QObject *parent = nullptr);

    ConnectionInfo info() const { return m_info; };
    void close();
    void sendData(const QByteArray &rawData);

    void updateLastActivity(); //опционально public. Activity обновляется при отправке, получении сообщений
signals:
    void messageReceived(const QUuid &sessionId, const QByteArray &rawData);
    void closed(const QUuid &sessionId);

private slots:
    void onReadyRead();
private:
    ConnectionInfo m_info;

    // void processIncomingMessage(const QByteArray &rawData);
    void closeSession();

    bool m_isClosed = false;
    QSslSocket* m_socket = nullptr;
    QByteArray m_buffer;
};

#endif // SOCKETWRAPPER_H
