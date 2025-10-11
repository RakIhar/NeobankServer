#include "applicationserver.h"
#include "QTcpServer"
#include <QDebug>

ApplicationServer::ApplicationServer(QObject *parent) : QObject(parent){}

ApplicationServer::~ApplicationServer()
{

};

bool ApplicationServer::initializeComponents()
{
    m_authManager = new AuthManager(this);
    m_dbManager = new DataBaseManager(this);
    m_sessionManager = new SessionManager(this);
    m_sslManager = new SSLManager(this);
    //m_logger = new SystemLogger(this);
    return true;
}



bool ApplicationServer::startServer()
{
    if(!m_isRunning && !initializeComponents())
        return false;

    return m_isRunning = true;
}
