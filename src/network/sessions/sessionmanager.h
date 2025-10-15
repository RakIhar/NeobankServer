#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QObject>
#include "clientsession.h"
#include <QPointer>
#include <QSet>
#include <QTcpSocket>
#include <QSslSocket>
#include <QPointer>

//Отвечает за жизненный цикл сессий.
//Создаёт ClientSession и хранит их.
//Подписывает сокет на read/write/error/disconnected в рамках конкретной сессии.

inline size_t qHash(const QPointer<ClientSession> &ptr, size_t seed = 0) noexcept {
    return qHash(ptr.data(), seed);
}

class SessionManager : public QObject
{
    Q_OBJECT
public:
    explicit SessionManager(QObject *parent = nullptr);
    void createSession(QPointer<QSslSocket> socket);
    void removeSession(ClientSession *session);
    int activeCount() const;
    QList<ClientSession*> sessions() const;
private:
    QSet<QPointer<ClientSession>> m_sessions;
    QTimer m_cleanupTimer;
private slots:
    void onSessionExpired(ClientSession *session);
    void cleanup();
signals:
};

#endif // SESSIONMANAGER_H
