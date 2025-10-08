#include "applicationserver.h"
#include "QTcpServer"
#include <QDebug>

ApplicationServer::ApplicationServer(QCoreApplication* app, QObject *parent) : QObject(parent), m_app(app){};

bool ApplicationServer::initializeComponents()
{
    this->m_TcpServer = QSharedPointer<QTcpServer>::create();
    this->m_authManager = QSharedPointer<AuthManager>::create();
    this->m_dbManager = QSharedPointer<DataBaseManager>::create();
    this->m_sessionManager = QSharedPointer<SessionManager>::create();
    this->m_sslManager = QSharedPointer<SSLManager>::create();
    this->m_logger = QSharedPointer<SystemLogger>::create();
    return true;
}

bool ApplicationServer::loadConfiguration()
{
    return true;
}

bool ApplicationServer::setupSignalHandlers()
{
    // запускаем сервер
    if (!m_TcpServer->listen(QHostAddress::Any, 8080)) {
        qCritical() << "Не удалось запустить сервер:" << m_TcpServer->errorString();
        return false;
    }
    qDebug() << "Сервер запущен на порту" << m_TcpServer->serverPort();

    connect(m_TcpServer.get(), &QTcpServer::newConnection, this, &ApplicationServer::onNewConnection);
    // bool onNewConnection();
    // bool handleClientDisconnected();
    // bool processClientMessage(const QByteArray& data, ClientSession* session);
    // connect()
    return true;
}

bool ApplicationServer::startServer()
{

    if (!initializeComponents())
        return false;

    if (!loadConfiguration())
        return false;

    if (!setupSignalHandlers())
        return false;
    // m_isRunning = true;
    return true;
}
