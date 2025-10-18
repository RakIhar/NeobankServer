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
#include "../../security/authentification/authmanager.h"

//Отвечает за жизненный цикл сессий.
//Создаёт ClientSession и хранит их.
//Подписывает сокет на read/write/error/disconnected в рамках конкретной сессии.

//Аутентификация:
//1 Аутентификация пользователя (покуда без двухфакторки)
//2 Создание SessionID
//3 Связывание SessionID + UserID в базе данных
//4 Передача SessionID клиенту
//5 Запись входа (опционально)

//При обращении к API:
//1 Проверить наличие и валидность(в бд) SessionID
//2 Проверить время бездействия (?)
//3 Одноразовые токены

//Завершение сессии:
//1 Логаут - кнопка выйти
//2 Таймаут
//3 Абсолютный таймаут
//4 Параллельный логин
//5 Принудительное завершение

//С клиентом сессия связывается по SSL, с БД сессия связывается по SessionID

class ClientSession : public QObject
{
    Q_OBJECT
public:
    explicit ClientSession(QSslSocket* socket, QObject *parent = nullptr);
private:
    QSslSocket* m_socket;
    QTimer m_timer;
    void sendData(const QByteArray &data);
    void processIncomingData(const QByteArray &data);
    void extendLifetime();

private slots:
    void onErrorOccurred(QAbstractSocket::SocketError socketError);
    void onReadyRead();
    void onSocketDisconnected();
    void onSocketDestroyed();
signals:
    void closed(ClientSession *self);
    void authenticated(QByteArray sessionID, ClientSession *self);
};

#endif // CLIENTSESSION_H
