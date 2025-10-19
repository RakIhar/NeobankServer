#include "sslservermanager.h"
#include <QFile>
#include <QSslKey>
#include <QSslCipher>
#include <QDebug>

SslServerManager::SslServerManager(QObject *parent)
    : QObject(parent)
{
    m_sessionManager = new SessionManager(this);
    m_sslServer = new QSslServer(this);

    connect(m_sslServer, &QSslServer::errorOccurred,
            this, &SslServerManager::onErrorOccurred);
    connect(m_sslServer, &QSslServer::sslErrors,
            this, &SslServerManager::onSslErrors);
    connect(m_sslServer, &QSslServer::startedEncryptionHandshake,
            this, &SslServerManager::onStartedEncryptionHandshake);

    //DEBUG//
    connect(m_sslServer, &QSslServer::newConnection,
            [this]{qDebug() << "newConnection";});
    connect(m_sslServer, &QSslServer::peerVerifyError,
            [this]{qDebug() << "peerVerifyError";});
    connect(m_sslServer, &QSslServer::handshakeInterruptedOnError,
            [this]{qDebug() << "handshakeInterruptedOnError";});
    connect(m_sslServer, &QSslServer::acceptError,
            [this]{qDebug() << "acceptError";});
    connect(m_sslServer, &QSslServer::startedEncryptionHandshake,
            [this]{qDebug() << "startedEncryptionHandshake";});

    initializeServerConfig();

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
    sslConfig.setPeerVerifyMode(QSslSocket::PeerVerifyMode::VerifyNone);
    QList<QSslCipher> ciphers {
                              QSslCipher("TLS_AES_256_GCM_SHA384"),
                              QSslCipher("TLS_AES_128_GCM_SHA256"),
                              QSslCipher("TLS_CHACHA20_POLY1305_SHA256")};

    sslConfig.setCiphers(ciphers);
    // sslConfig.setHandshakeMustInterruptOnError(false);
    m_sslServer->setSslConfiguration(sslConfig);
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

void SslServerManager::onStartedEncryptionHandshake(QSslSocket *socket)
{
    m_activeSockets.insert(socket);
    connect(socket, &QSslSocket::encrypted,
            this, &SslServerManager::onEncryptedReady);
    socket->setParent(this);
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
