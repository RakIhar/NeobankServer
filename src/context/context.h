#ifndef CONTEXT_H
#define CONTEXT_H
#include <QJsonObject>
#include "user.h"
#include "sessioncontext.h"
#include "../service/iservice.h"

class MessageContext;

class IEndpoint {
public:
    virtual ~IEndpoint() = default;

    virtual void invoke(MessageContext& ctx){};
};

class EndpointRegistry
{
public:
    void registerEndpoint(const QString& name, std::unique_ptr<IEndpoint> ep)
    { endpoints[name] = std::move(ep); }

    IEndpoint* getEndpoint(const QString& name)
    {
        auto it = endpoints.find(name);
        if (it == endpoints.end())
            return nullptr;

        return it->second.get();
    }
private:
    std::unordered_map<QString, std::unique_ptr<IEndpoint>> endpoints;
};

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
    User user;
    SessionContext session;
    QUuid requestId;
    ServiceScope services;
    QHash<QVariant, QVariant> items;
    EndpointRegistry& endpoints; //Владение endpoints у Application
    IEndpoint* currentEndpoint = nullptr; //без владения
};

#endif // CONTEXT_H
