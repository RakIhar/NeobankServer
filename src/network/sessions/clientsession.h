#ifndef CLIENTSESSION_H
#define CLIENTSESSION_H

#include <QObject>
#include <QPointer>
#include <QSet>
#include <QSsl>
#include <QSslSocket>
#include <QTcpSocket>
#include <QTimer>

#include "../../common/constants.h"
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

/*
Аутентификацию и авторизацию — хранение токена, ID пользователя, ролей.

Состояние соединения — активность, тайм-аут, идентификатор сокета.

Буферизацию данных — частичные запросы, фрагменты сообщений.

Очередь операций — команды, ожидающие выполнения или ответов.

Ошибки и флаги — состояние последней операции, причина обрыва.

Контекст транзакции — параметры бизнес-операций, текущие шаги.
*/

//Идентифицировать сессию пользователем?


struct SessionData
{
    QByteArray userID;
    QByteArray sessionID;
    bool isAuthenticated = false;
    Role role = Role::None;
    AuthMethod authMethod = AuthMethod::None;
    Permission permission = Permission::None;

};

/*
clientCertHash или tlsFingerprint — привязка к TLS-сеансу.
lastActivity (QDateTime) — для таймаута неактивности.
Таймер keep-alive.
sessionStartTime.
ipAddress, userAgent (если известны).
Буфер или очередь запросов, если протокол асинхронный.
accountContext / currentOperation (например, текущая транзакция).
locale или другие пользовательские параметры.

Ссылку на SessionManager для уведомления об окончании.
Ссылку на AuthManager или ProtocolHandler для делегирования логики.
*/

class ClientSession : public QObject
{
    Q_OBJECT
public:
    explicit ClientSession(QObject *parent, QPointer<QSslSocket> socket);
private:
    SessionData m_sData;

    QPointer<QSslSocket> m_socket;
    QTimer m_timer;

    bool sendData(const QByteArray &data);
    void processIncomingData(const QByteArray &data);

    bool isExpired() const;
    void extendLifetime();
private slots:
    void onErrorOccurred(QAbstractSocket::SocketError socketError);
    void onReadyRead();
    void onDisconnected();
    //newSessionTicketReceived
signals:
    void expired(ClientSession *self);
};
/*
private:

private slots:
    void onProtocolMessageReceived(const QByteArray &message); // слот для внутренней обработки
signals:
    void expired(ClientSession *self);
    void dataReceived(ClientSession *self, const QByteArray &data); // сигнал о полученных данных
    void dataSent(ClientSession *self, const QByteArray &data);     // сигнал об отправке данных
    void protocolError(ClientSession *self, const QString &error);  // сигнал ошибки протокола
};

*/
#endif // CLIENTSESSION_H
