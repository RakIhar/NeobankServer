#include "sslmanager.h"
#include <QFile>
#include <QDebug>
#include <QSslConfiguration>
#include <QSslCertificate>
#include <QSslKey>
#include <QList>
#include <QSslCipher>


//управление SSL/TLS соединениями

//QSslServer
// setSslConfiguration(const QSslConfiguration &cfg)                    Устанавливает параметры SSL (сертификат, ключ, шифры, CA и т.д.)
// sslConfiguration() const                                             Получить текущую конфигурацию
// setHandshakeTimeout(int ms)                                          Установить таймаут SSL-рукопожатия
// handshakeTimeout() const                                             Получить текущее значение таймаута
// Сигнал sslErrors(QSslSocket*, const QList<QSslError>&)               Ошибки сертификата клиента
// Сигнал peerVerifyError(QSslSocket*, const QSslError&)                Ошибка верификации клиента
// Сигнал startedEncryptionHandshake(QSslSocket*)                       Начало рукопожатия
// Сигнал handshakeInterruptedOnError(QSslSocket*, const QSslError&)	Ошибка в процессе установки канала
// Сигнал alertReceived / alertSent                                     Отладочная информация по TLS-алертам
// Сигнал errorOccurred                                                 Любая ошибка сокета
// Сигнал preSharedKeyAuthenticationRequired                            Для TLS-PSK (предварительно разделённый ключ)

//QTcpServer
// bool listen(const QHostAddress &address, quint16 port)               Запускает сервер на IP/порту. Возвращает false, если порт занят.
// void close()                                                         Закрывает сервер.
// bool isListening()                                                   Проверка, запущен ли сервер.
// QHostAddress serverAddress() / quint16 serverPort()                  Получить адрес и порт, на которых слушает сервер.
// QTcpSocket* nextPendingConnection()                                  Возвращает сокет для следующего ожидающего подключения.
// void pauseAccepting() / void resumeAccepting() (Qt 6+)               Приостановка / возобновление приёма новых соединений.
// Сигнал newConnection()                                               Вызывается, когда поступает новое подключение.
// Сигнал acceptError(QAbstractSocket::SocketError)                     Ошибка при приёме подключения.

//принять соединение и создать сессию в слоте
//удалить чета


//TLS_AES_256_GCM_SHA384
SSLManager::SSLManager(QObject *parent)
    : QObject(parent)
{
    // --- Загрузка сертификата ---
    QFile certFile("../../certificates/server.crt");
    QFile keyFile("../../certificates/server.key");
    if (!certFile.open(QIODevice::ReadOnly) || !keyFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Не удалось открыть сертификат или ключ";
        return;
    }

    QSslCertificate cert(&certFile, QSsl::Pem);
    QSslKey key(&keyFile, QSsl::Rsa, QSsl::Pem);

    // --- Базовая конфигурация ---
    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    sslConfig.setLocalCertificate(cert);
    sslConfig.setPrivateKey(key);

    // --- Протокол: только TLS 1.3 ---
    sslConfig.setProtocol(QSsl::TlsV1_3);

    // --- Шифронаборы TLS 1.3 ---
    QList<QSslCipher> ciphers;
    for (const auto &cipher : QSslConfiguration::supportedCiphers()) {
        if (cipher.name().startsWith("TLS_AES") ||
            cipher.name().startsWith("TLS_CHACHA20")) {
            ciphers.append(cipher);
        }
    }
    sslConfig.setCiphers(ciphers);

    // --- (опционально) проверка клиентов ---
    // sslConfig.setPeerVerifyMode(QSslSocket::VerifyPeer);

    // --- Применение конфигурации ---
    m_sslServer = new QSslServer(this);
    m_sslServer->setSslConfiguration(sslConfig);

    connect(m_sslServer, &QSslServer::newConnection,
            this, &SSLManager::onNewConnection);

    if (!m_sslServer->listen(QHostAddress::Any, 4433)) {
        qWarning() << "Ошибка запуска сервера:" << m_sslServer->errorString();
    } else {
        qInfo() << "TLS 1.3 сервер слушает на порту 4433";
    }
}

void SSLManager::onNewConnection()
{

}
