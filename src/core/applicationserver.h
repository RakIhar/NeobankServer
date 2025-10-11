#ifndef APPLICATIONSERVER_H
#define APPLICATIONSERVER_H

#include <QObject>
#include <QSharedPointer>
// #include "../network/sessions/clientsession.h"
#include "../network/sessions/sessionmanager.h"
#include "../database/dbmanager.h"
#include "../security/authentification/authmanager.h"
#include "../network/ssl/sslmanager.h"
#include <QCoreApplication>
#include <QTcpServer>


class ApplicationServer : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationServer(QObject *parent = nullptr);
    ~ApplicationServer();

    bool startServer();
    bool stopServer();
private:
    bool initializeComponents();

    SessionManager* m_sessionManager;
    DataBaseManager* m_dbManager;
    AuthManager* m_authManager;
    SSLManager* m_sslManager;

    bool m_isRunning = false;
};

#endif // APPLICATIONSERVER_H
