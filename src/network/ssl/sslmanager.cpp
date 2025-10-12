#include "sslmanager.h"
#include <QFile>
#include <QDebug>
#include <QSslConfiguration>
#include <QSslCertificate>
#include <QSslKey>
#include <QList>
#include <QSslCipher>

//полное управление SSL/TLS соединениями

void printConfig(QSslConfiguration sslConfig)
{
    qDebug() << "protocol:\n"
             << sslConfig.protocol();
    qDebug() << "localCertificate:\n"
             << sslConfig.localCertificate();
    qDebug() << "localCertificateChain:\n"
             << sslConfig.localCertificateChain();
    qDebug() << "privateKey:\n"
             << sslConfig.privateKey();
    qDebug() << "peerVerifyMode:\n"
             << sslConfig.peerVerifyMode();
    qDebug() << "peerVerifyDepth:\n"
             << sslConfig.peerVerifyDepth();
    qDebug() << "ciphers:\n"
             << sslConfig.ciphers();
    qDebug() << "supportedCiphers:\n"
             << sslConfig.supportedCiphers();
}

SslManager::SslManager(QObject *parent, SessionManager *sessionManager)
    : QObject(parent), m_sessionManager(sessionManager)
{
    initializeConfig();
    initializeServerSlots();

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
                              // QSslCipher("TLS_AES_128_GCM_SHA256"),
                              QSslCipher("TLS_CHACHA20_POLY1305_SHA256")};

    sslConfig.setCiphers(ciphers);

    m_sslServer = new QSslServer(this);
    m_sslServer->setSslConfiguration(sslConfig);
}

void SslManager::initializeServerSlots()
{
    connect(m_sslServer, &QSslServer::newConnection, this, &SslManager::onNewConnection);
    connect(m_sslServer, &QSslServer::acceptError, this, &SslManager::onAcceptError);
}

void SslManager::initializeSocketSlots(QSslSocket *sslSocket)
{
    connect(sslSocket, &QSslSocket::encrypted,
            this, &SslManager::onEncryptedReady, Qt::UniqueConnection);
    connect(sslSocket, &QSslSocket::sslErrors,
            this, &SslManager::onSslErrors, Qt::UniqueConnection);
    connect(sslSocket, &QSslSocket::errorOccurred,
            this, &SslManager::onSocketError, Qt::UniqueConnection);
    connect(sslSocket, &QSslSocket::handshakeInterruptedOnError,
            this, [](const QSslError &e){ qWarning() << "Handshake interrupted:" << e.errorString(); });
    connect(sslSocket, &QSslSocket::disconnected, this, [this, sslSocket]() {
        m_activeSockets.remove(sslSocket);
        sslSocket->deleteLater();
    });
}

void SslManager::onNewConnection()
{
    QTcpSocket *base = m_sslServer->nextPendingConnection();
    QSslSocket *ssl = qobject_cast<QSslSocket*>(base);
    if (!ssl)
        return;
    m_activeSockets.insert(ssl);
    initializeSocketSlots(ssl);
}

void SslManager::onEncryptedReady()
{
    QSslSocket *ssl = qobject_cast<QSslSocket*>(sender());
    if (!ssl)
        return;

    m_sessionManager->createSession(ssl);
}

void SslManager::onAcceptError(QAbstractSocket::SocketError error)
{
    //
}

void SslManager::onSslErrors(const QList<QSslError> &errors)
{
    for (const auto &e : errors)
        qWarning() << "SSL error:" << e.errorString();
}

void SslManager::onSocketError(QAbstractSocket::SocketError error)
{
    auto *ssl = qobject_cast<QSslSocket*>(sender());
    qWarning() << "Socket error:" << error
               << (ssl ? ssl->peerAddress().toString() : "unknown peer");
}
