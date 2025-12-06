#ifndef CONTEXT_H
#define CONTEXT_H
#include <QJsonObject>
#include "user.h"
#include "sessioncontext.h"
#include "../service/service.h"

class MessageContext;

class IEndpoint {
public:
    virtual ~IEndpoint() = default;

    virtual void invoke(MessageContext& ctx);
};

class EndpointRegistry
{
public:
    void registerEndpoint(const QString& name, QSharedPointer<IEndpoint> ep)
    { endpoints[name] = ep; }

    QSharedPointer<IEndpoint> getEndpoint(const QString& name)
    { return endpoints.value(name, nullptr); }

private:
    QHash<QString, QSharedPointer<IEndpoint>> endpoints;
};


class MessageContext
{
public:
    MessageContext(ServiceScope services, EndpointRegistry& endpoints)
        : services(std::move(services))
        , endpoints(endpoints)
    {}
    bool aborted = false;
    void abort() { aborted = true; }
    QJsonObject jsonRequest;
    QJsonObject jsonResponce;
    QUuid connection;
    User user;
    SessionContext session;
    QUuid requestId;
    ServiceScope services;
    QHash<QVariant, QVariant> items;
    EndpointRegistry& endpoints; //Владение endpoints у Application
    IEndpoint* currentEndpoint = nullptr; //без владения
};

#endif // CONTEXT_H
