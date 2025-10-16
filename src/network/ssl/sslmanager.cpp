#include "sslmanager.h"
#include <QFile>
#include <QDebug>
#include <QSslConfiguration>
#include <QSslCertificate>
#include <QSslKey>
#include <QList>
#include <QSslCipher>

SslManager::SslManager(QObject *parent, SessionManager *sessionManager)
    : QObject(parent), m_sessionManager(sessionManager)
{
    initializeConfig();

    if (!m_sslServer->listen(QHostAddress::Any, 4433)) { //настроить сами порты и прочее
        qWarning() << "Ошибка запуска сервера:" << m_sslServer->errorString();
    } else {
        qInfo() << "TLS 1.3 сервер слушает на порту 4433";
    }
}

void SslManager::initializeConfig()
{
    QFile certFile("../../certificates/server.crt");
    QFile keyFile("../../certificates/server.key");

    if (!certFile.open(QIODevice::ReadOnly) || !keyFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Не удалось открыть сертификат или ключ"; //лучше исключение
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

void SslManager::initializeServerSlots()
{
    connect(m_sslServer, &QSslServer::newConnection,
            this, &SslManager::onNewConnection);
    connect(m_sslServer, &QSslServer::errorOccurred,
            this, &SslManager::onErrorOccurred);
    connect(m_sslServer, &QSslServer::acceptError,
            this, &SslManager::onAcceptError);
    connect(m_sslServer, &QSslServer::handshakeInterruptedOnError,
            this, &SslManager::onHandshakeInterruptedOnError);
    connect(m_sslServer, &QSslServer::peerVerifyError,
            this, &SslManager::onPeerVerifyError);
    connect(m_sslServer, &QSslServer::sslErrors,
            this, &SslManager::onSslErrors);
}

//===========================================================================//

void SslManager::onNewConnection()
{
    QTcpSocket *base = m_sslServer->nextPendingConnection();
    QSslSocket *ssl = qobject_cast<QSslSocket*>(base);
    if (!ssl)
        return;
    m_activeSockets.insert(ssl);
    connect(ssl, &QSslSocket::encrypted,
            this, &SslManager::onEncryptedReady, Qt::UniqueConnection);
}

void SslManager::onEncryptedReady()
{
    QSslSocket *ssl = qobject_cast<QSslSocket*>(sender());
    if (!ssl)
        return;

    m_sessionManager->createSession(ssl);
    m_activeSockets.remove(ssl);
}

void SslManager::disconnectAll()
{

}

//===========================================================================//

void SslManager::onAcceptError(QAbstractSocket::SocketError socketError)
{
    qWarning() << "Accept error: " << socketError;
}

void SslManager::onSslErrors(QSslSocket *socket, const QList<QSslError> &errors)
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

void SslManager::onErrorOccurred(QSslSocket *socket, QAbstractSocket::SocketError error)
{
    qWarning() << "Error occurred: " << error
               << (socket ? socket->peerAddress().toString() : "unknown peer");
}

void SslManager::onHandshakeInterruptedOnError(QSslSocket *socket, const QSslError &error)
{
    //можно обработать
    //должен быть Qt::DirectConnection
    //sslConfig.setHandshakeMustInterruptOnError(false);

    qWarning() << "Handshake interrupted on error: " << error.errorString();
    // socket->
}

void SslManager::onPeerVerifyError(QSslSocket *socket, const QSslError &error)
{
    //можно заранее обработать, а иначе вызовется SslErrors
    qWarning() << "Peer verify error: " << error.errorString();
    // socket->
}
