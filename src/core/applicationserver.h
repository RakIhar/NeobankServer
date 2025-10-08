#ifndef APPLICATIONSERVER_H
#define APPLICATIONSERVER_H

#include <QObject>
#include <QSharedPointer>
#include "../network/sessions/clientsession.h"
#include "../network/sessions/sessionmanager.h"
#include "../database/dbmanager.h"
#include "../security/authentification/authmanager.h"
#include "../network/ssl/sslmanager.h"
#include "../network/core/tcpserver.h"
#include <QCoreApplication>
#include <QTcpServer>
//dependency injection ?? - указатели на менеджеров

class ApplicationServer : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationServer(QCoreApplication* app, QObject *parent = nullptr);
    ~ApplicationServer();

    bool startServer();
    bool stopServer();
private:
    bool initializeComponents();
    bool loadConfiguration();
    bool setupSignalHandlers();

    QSharedPointer<TcpServer> m_tcpServer;
    QSharedPointer<SessionManager> m_sessionManager;
    QSharedPointer<DataBaseManager> m_dbManager;
    QSharedPointer<AuthManager> m_authManager;
    QSharedPointer<SSLManager> m_sslManager;
    // QSharedPointer<SystemLogger> m_logger;

    bool m_isRunning;
    QCoreApplication* m_app;

private slots:
    bool onNewConnection();
    bool handleClientDisconnected();
    bool processClientMessage(const QByteArray& data, ClientSession* session);
};

#endif // APPLICATIONSERVER_H
