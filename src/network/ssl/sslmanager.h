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
};

#endif // SSLMANAGER_H
