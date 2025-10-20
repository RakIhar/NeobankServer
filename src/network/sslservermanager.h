#ifndef SSLSERVERMANAGER_H
#define SSLSERVERMANAGER_H

#include <QObject>
#include <QSslServer>
#include <QSsl>
#include "sessions/sessionmanager.h"
#include <QSslSocket>

#include <QPointer>
#include <QSslSocket>
#include <QtGlobal>

// Прослушивает порт, обрабатывает handshake,
// передаёт зашифрованные сокеты в SessionManager

inline size_t qHash(const QPointer<QSslSocket> &ptr, size_t seed = 0) noexcept
{
    return qHash(reinterpret_cast<quintptr>(ptr.data()), seed);
}

class SslServerManager : public QObject
{
    Q_OBJECT
public:
    explicit SslServerManager(QObject *parent = nullptr);
    void startServer();
    void stopServer();
private slots:
    void onEncryptedReady();
    void cleanupDeadSockets();

    void onSslErrors(QSslSocket *socket, const QList<QSslError> &errors);
    void onErrorOccurred(QSslSocket *socket, QAbstractSocket::SocketError error);
    void onStartedEncryptionHandshake(QSslSocket *socket);
private:
    QTimer m_cleanupTimer; // Нужно ли
    QSslServer* m_sslServer;
    SessionManager* m_sessionManager;
    QSet<QPointer<QSslSocket>> m_activeSockets;

    void initializeServerConfig();

    void disconnectAll();
};

#endif // SSLSERVERMANAGER_H
