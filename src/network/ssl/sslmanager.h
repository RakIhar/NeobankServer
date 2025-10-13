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
    void onAcceptError(QAbstractSocket::SocketError error);

    void onEncryptedReady();
    void onSslErrors(const QList<QSslError> &errors);
    void onSocketError(QAbstractSocket::SocketError error);

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
