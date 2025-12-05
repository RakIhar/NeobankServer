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

    virtual void invoke(MessageContext& ctx) = 0;
};

class EndpointRegistry
{
public:
    void registerEndpoint(const QString& name, QSharedPointer<IEndpoint> ep)
    {
        endpoints[name] = ep;
    }

    QSharedPointer<IEndpoint> getEndpoint(const QString& name)
    {
        return endpoints.value(name, nullptr);
    }

private:
    QHash<QString, QSharedPointer<IEndpoint>> endpoints;
};


class MessageContext
{
public:
    MessageContext();
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
    EndpointRegistry& endpoints;
    QSharedPointer<IEndpoint> currentEndpoint;
};

#endif // CONTEXT_H
