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
    void onSslErrors(const QList<QSslError> &errors);
    void onErrorOccurred(QAbstractSocket::SocketError error);

private:
    QSslServer* m_sslServer;
    SessionManager* m_sessionManager;
    QSet<QPointer<QSslSocket>> m_activeSockets;

    void initializeConfig();
    void initializeServerSlots();
    void initializeSocketSlots(QPointer<QSslSocket> sslSocket);
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
