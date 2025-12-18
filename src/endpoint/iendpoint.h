#ifndef IENDPOINT_H
#define IENDPOINT_H
#include <QString>
#include <QHash>

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

#endif // IENDPOINT_H
