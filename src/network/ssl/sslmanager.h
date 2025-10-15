#ifndef SSLMANAGER_H
#define SSLMANAGER_H

#include <QObject>
#include <QSslServer>
#include <QSsl>
#include "../sessions/sessionmanager.h"
#include <QSslSocket>

#include <QPointer>
#include <QSslSocket>
#include <QtGlobal>

// Отвечает за прослушивание порта, создание сокета и handshake.
// Обрабатывает только TLS‑ошибки и encrypted.
// Может накинуть методов для очистки, защиты от Dos, DDoS

inline size_t qHash(const QPointer<QSslSocket> &ptr, size_t seed = 0) noexcept
{
    return qHash(reinterpret_cast<quintptr>(ptr.data()), seed);
}

class SslManager : public QObject
{
    Q_OBJECT
public:
    explicit SslManager(QObject *parent, SessionManager *sessionManager);

private slots:
    void onNewConnection();
    void onEncryptedReady();
    void onSslErrors(QSslSocket *socket, const QList<QSslError> &errors);
    void onErrorOccurred(QSslSocket *socket, QAbstractSocket::SocketError error);
    void onAcceptError(QAbstractSocket::SocketError socketError);
    void onHandshakeInterruptedOnError(QSslSocket *socket, const QSslError &error);
    void onPeerVerifyError(QSslSocket *socket, const QSslError &error);
private:
    QSslServer* m_sslServer;
    SessionManager* m_sessionManager;
    QSet<QPointer<QSslSocket>> m_activeSockets;

    void initializeConfig();
    void initializeServerSlots();
    void initializeSocketSlots(QPointer<QSslSocket> sslSocket);
    void disconnectAll();

    ClientSession* sessionForSocket(QPointer<QSslSocket> socket) const;
signals:
};

#endif // SSLMANAGER_H

/*
QSslServer:
alertReceived
скип
alertSent
скип
acceptError(QAbstractSocket::SocketError socketError)
ошибка подключения - уровень TcpServer. Сокет не создается
errorOccurred
ошибка на протяжении рукопожатия. Сокет уничтожается
handshakeInterruptedOnError
ошибка проверки сертификата. Можно продолжить
peerVerifyError
ошибка при верификации. Если ничего не делать, то вызовется sslErrors
sslErrors
Список ошибок после рукопожатия, можно обработать

QSslSocket:
handshakeInterruptedOnError
то же самое, что и в сервере
peerVerifyError
то же самое, что и в сервере
sslErrors
то же самое, что и в сервере
errorOccurred
уровень AbstractSocket. Ошибка на протяжение передачи или рукопожатия
*/
