#include "sslservermanager.h"
#include <QFile>
#include <QSslKey>
#include <QSslCipher>

SslServerManager::SslServerManager(QObject *parent)
    : QObject(parent)
{
    m_sessionManager = new SessionManager(this);
    initializeServerConfig();
    initializeServerSlots();

    constexpr int ONE_MINUTE = 60000;
    m_cleanupTimer.setInterval(ONE_MINUTE);
    m_cleanupTimer.setSingleShot(false);
    connect(&m_cleanupTimer, &QTimer::timeout, this, &SslServerManager::cleanupDeadSockets);
    m_cleanupTimer.start();
}

void SslServerManager::initializeServerConfig()
{
    QFile certFile("../../certificates/server.crt");
    QFile keyFile("../../certificates/server.key");

    if (!certFile.open(QIODevice::ReadOnly) || !keyFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Не удалось открыть сертификат или ключ";
        return;
    }

    QSslCertificate cert(&certFile, QSsl::Pem); //X.509
    QSslKey key(&keyFile, QSsl::Rsa, QSsl::Pem);
    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    sslConfig.setLocalCertificate(cert);
    sslConfig.setPrivateKey(key);

    sslConfig.setProtocol(QSsl::SslProtocol::TlsV1_3);
    sslConfig.setPeerVerifyMode(QSslSocket::PeerVerifyMode::VerifyPeer);
    QList<QSslCipher> ciphers {
                              QSslCipher("TLS_AES_256_GCM_SHA384"),
                              QSslCipher("TLS_AES_128_GCM_SHA256"),
                              QSslCipher("TLS_CHACHA20_POLY1305_SHA256")};

    sslConfig.setCiphers(ciphers);

    sslConfig.setHandshakeMustInterruptOnError(false);

    m_sslServer = new QSslServer(this);
    m_sslServer->setSslConfiguration(sslConfig);
}

void SslServerManager::initializeServerSlots()
{
    connect(m_sslServer, &QSslServer::newConnection,
            this, &SslServerManager::onNewConnection);
    connect(m_sslServer, &QSslServer::errorOccurred,
            this, &SslServerManager::onErrorOccurred);
    connect(m_sslServer, &QSslServer::acceptError,
            this, &SslServerManager::onAcceptError);
    connect(m_sslServer, &QSslServer::handshakeInterruptedOnError,
            this, &SslServerManager::onHandshakeInterruptedOnError);
    connect(m_sslServer, &QSslServer::peerVerifyError,
            this, &SslServerManager::onPeerVerifyError);
    connect(m_sslServer, &QSslServer::sslErrors,
            this, &SslServerManager::onSslErrors);
}

//===========================================================================//

void SslServerManager::startServer()
{
    if (!m_sslServer->listen(QHostAddress::Any, 4433)) { //настроить сами порты и прочее
        qWarning() << "Ошибка запуска сервера:" << m_sslServer->errorString();
    } else {
        qInfo() << "TLS 1.3 сервер слушает на порту 4433";
    }
}

void SslServerManager::stopServer()
{
    m_sslServer->close();
}

void SslServerManager::disconnectAll()
{

}

//===========================================================================//

void SslServerManager::onNewConnection()
{
    QTcpSocket *baseSocket = m_sslServer->nextPendingConnection();
    QSslSocket *sslSocket = qobject_cast<QSslSocket*>(baseSocket);
    if (!sslSocket)
        return;
    m_activeSockets.insert(sslSocket);
    connect(sslSocket, &QSslSocket::encrypted,
            this, &SslServerManager::onEncryptedReady, Qt::UniqueConnection);
    sslSocket->setParent(this);
}

void SslServerManager::onEncryptedReady()
{
    QSslSocket *ssl = qobject_cast<QSslSocket*>(sender());
    if (!ssl)
        return;

    m_sessionManager->createUnauthenticatedSession(ssl);
    m_activeSockets.remove(ssl);
}

void SslServerManager::cleanupDeadSockets()
{
    auto it = m_activeSockets.begin();
    while (it != m_activeSockets.end())
    {
        if (*it == nullptr)
        {
            it = m_activeSockets.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

//===========================================================================//

void SslServerManager::onAcceptError(QAbstractSocket::SocketError socketError)
{
    qWarning() << "Accept error: " << socketError;
}

void SslServerManager::onSslErrors(QSslSocket *socket, const QList<QSslError> &errors)
{
    bool isThereCriticalError = false;
    for (const auto &e : errors)
    {
        switch (e.error()) {
        case QSslError::SelfSignedCertificate:
        case QSslError::SelfSignedCertificateInChain:
        case QSslError::UnableToGetLocalIssuerCertificate:
        case QSslError::UnableToVerifyFirstCertificate:
        case QSslError::HostNameMismatch:
        case QSslError::CertificateUntrusted:
        case QSslError::OcspNoResponseFound:
            break;
        default:
            isThereCriticalError = true;
            break;
        }

        qWarning() << "SSL error: " << e.errorString();
    }
    if (isThereCriticalError)
    {
        qWarning() << "Критическая SSL ошибка — соединение разорвано";
        socket->abort();
    }
    else
    {
        socket->ignoreSslErrors();
    }
}

void SslServerManager::onErrorOccurred(QSslSocket *socket, QAbstractSocket::SocketError error)
{
    qWarning() << "Error occurred: " << error
               << (socket ? socket->peerAddress().toString() : "unknown peer");
}

void SslServerManager::onHandshakeInterruptedOnError(QSslSocket *socket, const QSslError &error)
{
    //можно обработать
    //должен быть Qt::DirectConnection
    //sslConfig.setHandshakeMustInterruptOnError(false);

    qWarning() << "Handshake interrupted on error: " << error.errorString();
    // socket->
}

void SslServerManager::onPeerVerifyError(QSslSocket *socket, const QSslError &error)
{
    //можно заранее обработать, а иначе вызовется SslErrors
    qWarning() << "Peer verify error: " << error.errorString();
    // socket->
}
