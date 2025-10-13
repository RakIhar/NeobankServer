#ifndef CLIENTSESSION_H
#define CLIENTSESSION_H

#include <QObject>
#include <QPointer>
#include <QSet>
#include <QSsl>
#include <QSslSocket>
#include <QTcpSocket>
#include <QTimer>

//QTcpSocket
// void connectToHost(const QString &host, quint16 port)    Подключиться к серверу (для клиента).
// void disconnectFromHost()                                Закрыть соединение корректно.

// qint64 write(const QByteArray &data)                     Отправить данные.
// QByteArray readAll() / read(qint64 maxSize)              Прочитать данные.
// qint64 bytesAvailable()                                  Проверить, сколько байт можно прочитать.
// qint64 bytesToWrite()                                    Сколько данных ещё не отправлено.
// QHostAddress peerAddress() / peerPort()                  Адрес и порт клиента.
// QHostAddress localAddress() / localPort()                Адрес и порт сокета на локальной стороне.
// Сигнал readyRead()                                       Срабатывает, когда пришли данные.
// Сигнал connected()                                       Сокет подключился (для клиента).
// Сигнал disconnected()                                    Соединение закрыто.
// Сигнал errorOccurred(QAbstractSocket::SocketError)       Ошибка.

//QSslSocket
// Сигнал sslErrors(const QList<QSslError> &errors)         Ошибка проверки сертификата
// Сигнал peerVerifyError(const QSslError &error)           Ошибка верификации пира (например, неверный CN)

/*
Аутентификацию и авторизацию — хранение токена, ID пользователя, ролей.

Состояние соединения — активность, тайм-аут, идентификатор сокета.

Буферизацию данных — частичные запросы, фрагменты сообщений.

Очередь операций — команды, ожидающие выполнения или ответов.

Ошибки и флаги — состояние последней операции, причина обрыва.

Контекст транзакции — параметры бизнес-операций, текущие шаги.
*/

class ClientSession : public QObject
{
    Q_OBJECT
public:
    explicit ClientSession(QObject *parent, QPointer<QSslSocket> socket);
private:
    QPointer<QSslSocket> m_socket;
    QTimer m_timer;
signals:
    void expired(ClientSession *self);
};

#endif // CLIENTSESSION_H
