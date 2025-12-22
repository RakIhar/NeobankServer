#include "transportlayer.h"
#include "sessionmanager.h"
#include <QFile>
#include <QSslKey>
#include <QSslCipher>
#include <qjsondocument.h>

TransportLayer::TransportLayer(QObject *parent)
    : QObject(parent)
{
    m_sessionManager = new ConnectionManager(this);
    connect(m_sessionManager, &ConnectionManager::closed, this, [this](const QUuid &connection){
        emit closed(connection);
    });
    connect(m_sessionManager, &ConnectionManager::messageReceived, this, [this](const QUuid &connection, const QByteArray &rawData){       
        QJsonDocument doc = QJsonDocument::fromJson(rawData);
        if (!doc.isObject()) return;

        if (!doc.isNull()) {
            qDebug().noquote() << "Received JSON:\n" << doc.toJson(QJsonDocument::Indented);
        } else
            qDebug() << "Received:\n" << rawData;

        emit messageReceived(connection, rawData);
    });

    m_sslServer = new QSslServer(this);
    connect(m_sslServer, &QSslServer::errorOccurred, this,[this]
        (QSslSocket *socket, QAbstractSocket::SocketError error){
            qWarning() << "Error occurred: " << error << (socket ? socket->peerAddress().toString() : "unknown peer");
    });
    connect(m_sslServer, &QSslServer::sslErrors,
            this, &TransportLayer::onSslErrors);
    connect(m_sslServer, &QSslServer::startedEncryptionHandshake,
            this, &TransportLayer::onStartedEncryptionHandshake);

    initializeServerConfig();

    constexpr int ONE_MINUTE = 60000;
    m_cleanupTimer.setInterval(ONE_MINUTE);
    m_cleanupTimer.setSingleShot(false);
    connect(&m_cleanupTimer, &QTimer::timeout, this, &TransportLayer::cleanup);
    m_cleanupTimer.start();
}

void TransportLayer::initializeServerConfig()
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
    m_sslServer->setSslConfiguration(sslConfig);
}

void TransportLayer::start()
{
    constexpr quint16 PORT = 4433;
    if (!m_sslServer->listen(QHostAddress::Any, PORT)) {
        qWarning() << "Ошибка запуска сервера:" << m_sslServer->errorString();
    } else {
        qInfo() << QString("TLS 1.3 сервер слушает на порту %1").arg(PORT);
    }
}

void TransportLayer::stop()
{
    m_sslServer->close();
}

void TransportLayer::close(const QUuid connection)
{
    m_sessionManager->close(connection);
}

void TransportLayer::sendMessage(const QUuid connection, const QByteArray rawData)
{
    QJsonDocument doc = QJsonDocument::fromJson(rawData);
    if (!doc.isObject()) return;

    if (!doc.isNull()) {
        qDebug().noquote() << "Sent JSON:\n" << doc.toJson(QJsonDocument::Indented);
    } else
        qDebug() << "Sent:\n" << rawData;

    m_sessionManager->sendMessage(connection, rawData);
}

void TransportLayer::onStartedEncryptionHandshake(QSslSocket *socket)
{
    m_activeSockets.insert(socket);
    connect(socket, &QSslSocket::encrypted,
            this, &TransportLayer::onEncryptedReady);
    socket->setParent(this);
}

void TransportLayer::onEncryptedReady()
{
    qDebug() << "Подключено";
    QSslSocket *ssl = qobject_cast<QSslSocket*>(sender());
    if (!ssl)
        return;

    m_sessionManager->CreateConnection(ssl);
    m_activeSockets.remove(ssl);
}

void TransportLayer::cleanup()
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

void TransportLayer::onSslErrors(QSslSocket *socket, const QList<QSslError> &errors)
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
