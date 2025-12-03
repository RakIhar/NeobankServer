#ifndef MESSAGECONTEXT_H
#define MESSAGECONTEXT_H
#include <QJsonObject>
#include "user.h"
#include "session.h"

class MessageContext
{
public:
    MessageContext();

    QJsonObject jsonRequest;

    QJsonObject jsonResponce;

    QUuid connection;

    User user;

    Session session;

    QUuid requestId;

    //сервисы и данные (можно получить в middleware, контроллерах)
    //IServiceProvider requestServices
    //IDictionary<object, object> items //хранилище на время запроса

    //features
    //IFeatureCollection

    //CancellationToken RequestAborted

    //EndPoint

    //Abort()
};

#endif // MESSAGECONTEXT_H
