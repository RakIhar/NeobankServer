#ifndef MESSAGECONTEXT_H
#define MESSAGECONTEXT_H
#include <QJsonObject>
#include "user.h"
#include "sessioncontext.h"
#include "../service/iservice.h"
#include "../endpoint/iendpoint.h"

class MessageContext
{
public:
    MessageContext(ServiceScope services, EndpointRegistry& endpoints)
        : services(std::move(services))
        , endpoints(endpoints)
    {}
    bool isAborted = false;
    void abort() { isAborted = true; }
    QJsonObject jsonRequest;
    QJsonObject jsonResponce;
    QUuid connection;
    Context::User user;
    Context::Session session;
    QUuid requestId;
    ServiceScope services;
    QHash<QString, QVariant> items;
    EndpointRegistry& endpoints; //Владение endpoints у Application
    IEndpoint* currentEndpoint = nullptr; //без владения
};



#endif // MESSAGECONTEXT_H
