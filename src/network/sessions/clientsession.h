#ifndef CLIENTSESSION_H
#define CLIENTSESSION_H

#include <QObject>
#include <QPointer>
#include <QSet>
#include <QSsl>
#include <QSslSocket>
#include <QTcpSocket>

//сама сессия - оболочка над Tcp
//QTcpSocket
// void connectToHost(const QString &host, quint16 port)                Подключиться к серверу (для клиента).
// void disconnectFromHost()                                            Закрыть соединение корректно.
// bool waitForConnected(int msec)                                      Синхронное ожидание подключения (реже используется).
// qint64 write(const QByteArray &data)                                 Отправить данные.
// QByteArray readAll() / read(qint64 maxSize)                          Прочитать данные.
// qint64 bytesAvailable()                                              Проверить, сколько байт можно прочитать.
// qint64 bytesToWrite()                                                Сколько данных ещё не отправлено.
// QHostAddress peerAddress() / peerPort()                              Адрес и порт клиента.
// QHostAddress localAddress() / localPort()                            Адрес и порт сокета на локальной стороне.
// Сигнал readyRead()                                                   Срабатывает, когда пришли данные.
// Сигнал connected()                                                   Сокет подключился (для клиента).
// Сигнал disconnected()                                                Соединение закрыто.
// Сигнал errorOccurred(QAbstractSocket::SocketError)                   Ошибка.

//QSslSocket
// void connectToHostEncrypted(const QString &hostName, quint16 port)	Подключиться к серверу с TLS
// void startClientEncryption()                                         Запустить шифрование вручную (для клиента)
// void startServerEncryption()                                         Запустить шифрование вручную (для сервера)
// bool isEncrypted() const                                             Проверяет, установлено ли зашифрованное соединение
// void setPrivateKey(const QSslKey &key)                               Устанавливает приватный ключ
// void setLocalCertificate(const QSslCertificate &certificate)         Сертификат сервера
// void setCaCertificates(const QList<QSslCertificate> &certs)          Сертификаты доверенных центров (CA)
// QSslCipher sessionCipher() const                                     Получить используемый алгоритм шифрования
// QSslCertificate peerCertificate() const                              Сертификат другой стороны (сервер/клиент)
// Сигнал encrypted()                                                   Соединение успешно зашифровано
// Сигнал sslErrors(const QList<QSslError> &errors)                     Ошибка проверки сертификата
// Сигнал peerVerifyError(const QSslError &error)                       Ошибка верификации пира (например, неверный CN)

class ClientSession : public QObject
{
    Q_OBJECT
public:
    explicit ClientSession(QObject *parent = nullptr);
signals:
};

#endif // CLIENTSESSION_H
