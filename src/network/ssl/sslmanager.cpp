#include "sslmanager.h"
#include <QFile>
#include <QDebug>
#include <QSslConfiguration>
#include <QSslCertificate>
#include <QSslKey>
#include <QList>
#include <QSslCipher>

//управление SSL/TLS соединениями

/*
QSslServer:
alertReceived
скип
alertSent
скип
acceptError(QAbstractSocket::SocketError socketError)
ошибка подключения - уровень TcpServer. Сокет не создается
errorOccurred
ошибка на протяжении рукопожатия. Сокет уничтожается
handshakeInterruptedOnError
ошибка проверки сертификата. Можно продолжить
peerVerifyError
ошибка при верификации. Если ничего не делать, то вызовется sslErrors
sslErrors
Список ошибок после рукопожатия, можно обработать

QSslSocket:
handshakeInterruptedOnError
то же самое, что и в сервере
peerVerifyError
то же самое, что и в сервере
sslErrors
то же самое, что и в сервере
errorOccurred
уровень AbstractSocket. Ошибка на протяжение передачи или рукопожатия
*/

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

    sslConfig.setHandshakeMustInterruptOnError(false);

    m_sslServer = new QSslServer(this);
    m_sslServer->setSslConfiguration(sslConfig);
}

void SslManager::initializeServerSlots()
{
    connect(m_sslServer, &QSslServer::newConnection, this, &SslManager::onNewConnection);
    connect(m_sslServer, &QSslServer::errorOccurred, this, &SslManager::onErrorOccurred);
    connect(m_sslServer, &QSslServer::acceptError, this, &SslManager::onAcceptError);
    connect(m_sslServer, &QSslServer::handshakeInterruptedOnError, this, &SslManager::onHandshakeInterruptedOnError);
    connect(m_sslServer, &QSslServer::peerVerifyError, this, &SslManager::onPeerVerifyError);
    connect(m_sslServer, &QSslServer::sslErrors, this, &SslManager::onSslErrors);
}

void SslManager::initializeSocketSlots(QPointer<QSslSocket> sslSocket)
{
    connect(sslSocket, &QSslSocket::encrypted,
            this, &SslManager::onEncryptedReady, Qt::UniqueConnection);
}

void SslManager::disconnectAll()
{

}

ClientSession *SslManager::sessionForSocket(QPointer<QSslSocket> socket) const
{

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
    m_activeSockets.remove(ssl);
}

void SslManager::onAcceptError(QAbstractSocket::SocketError socketError)
{
    //обработать нельзя, прервано TCL соединение
    qWarning() << "Accept error: " << socketError; //добавить лог
}

//socket->ignoreSslErrors({error});
//socket->abort()

void SslManager::onSslErrors(QSslSocket *socket, const QList<QSslError> &errors)
{
    for (const auto &e : errors)
    {
        qWarning() << "SSL error: " << e.errorString(); //добавить лог
        // socket->
    }
}

void SslManager::onErrorOccurred(QSslSocket *socket, QAbstractSocket::SocketError error)
{
    //обработать нельзя, ошибка на handshake
    qWarning() << "Error occurred: " << error
               << (socket ? socket->peerAddress().toString() : "unknown peer"); //добавить лог
    // socket->
}

void SslManager::onHandshakeInterruptedOnError(QSslSocket *socket, const QSslError &error)
{
    //можно обработать
    //должен быть Qt::DirectConnection
    //sslConfig.setHandshakeMustInterruptOnError(false);
    qWarning() << "Handshake interrupted on error: " << error.errorString(); //добавить лог
    // socket->
}

void SslManager::onPeerVerifyError(QSslSocket *socket, const QSslError &error) //добавить лог
{
    //можно заранее обработать, а иначе вызовется SslErrors
    qWarning() << "Peer verify error: " << error.errorString();
    // socket->
}
