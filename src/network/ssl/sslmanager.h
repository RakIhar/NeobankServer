#ifndef SSLMANAGER_H
#define SSLMANAGER_H

#include <QObject>
#include <QSslServer> //обертка над QTcpServer
#include <QSsl> //набор SSL констант

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

class SSLManager : public QObject
{
    Q_OBJECT
public:
    explicit SSLManager(QObject *parent = nullptr);

private slots:
    void onNewConnection();

private:
    QSslServer* m_sslServer;
signals:
};

#endif // SSLMANAGER_H
