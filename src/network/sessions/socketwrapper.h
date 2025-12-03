#ifndef SOCKETWRAPPER_H
#define SOCKETWRAPPER_H

#include <QSslSocket>
#include <QUuid>

class SocketWrapper : public QObject
{
    Q_OBJECT
public:
    explicit SocketWrapper(const QUuid &sessionId,
                           QSslSocket* socket,
                           QObject *parent = nullptr);

    QUuid id() const noexcept { return m_sessionId; }
    void terminate();
    void sendData(const QByteArray &msg);
signals:
    void messageReceived(const QUuid &sessionId, const QByteArray &msg);
    void closed(const QUuid &sessionId);
private:

private slots:
    void onReadyRead();
private:
    void processIncomingMessage(const QByteArray &msg);
    void closeSession();
    const QUuid m_sessionId;
    bool m_isClosed = false;
    QSslSocket* m_socket = nullptr;
    QByteArray m_buffer;
};

#endif // SOCKETWRAPPER_H
