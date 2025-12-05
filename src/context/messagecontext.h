#ifndef MESSAGECONTEXT_H
#define MESSAGECONTEXT_H
#include <QJsonObject>
#include "user.h"
#include "sessioncontext.h"

class MessageContext
{
public:
    MessageContext();

    QJsonObject jsonRequest;

    QJsonObject jsonResponce;

    QUuid connection;

    User user;

    SessionContext session;

    QUuid requestId;



    /*
    //сервисы и данные (можно получить в middleware, контроллерах)
    //IServiceProvider requestServices
    //IDictionary<object, object> items //хранилище на время запроса
    //features
    //IFeatureCollection
    //CancellationToken RequestAborted
    //EndPoint
    //Abort()

    Connection: представляет информацию о подключении, которое установлено для данного запроса

    Features: получает коллекцию HTTP-функциональностей, которые доступны для этого запроса

    Items: получает или устанавливает коллекцию пар ключ-значение для хранения некоторых данных для текущего запроса

    Request: возвращает объект HttpRequest, который хранит информацию о текущем запросе

    RequestAborted: уведомляет приложение, когда подключение прерывается, и соответственно обработка запроса должна быть отменена

    RequestServices: получает или устанавливает объект IServiceProvider, который предоставляет доступ к контейнеру сервисов запроса

    Response: возвращает объект HttpResponse, который позволяет управлять ответом клиенту

    Session: хранит данные сессии для текущего запроса

    TraceIdentifier: представляет уникальный идентификатор запроса для логов трассировки

    User: представляет пользователя, ассоциированного с этим запросом

    WebSockets: возвращает объект для управления подключениями WebSocket для данного запроса
    */

};

#endif // MESSAGECONTEXT_H
