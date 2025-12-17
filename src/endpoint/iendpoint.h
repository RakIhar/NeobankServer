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

/*
#include <functional>
#include <memory>
#include <QDebug>

class IService
{
public:
    IService() = default;
    virtual ~IService() = default;
};

struct IServiceProvider {
    virtual IService* getRaw(size_t key) = 0;
};

using Factory = std::function<std::unique_ptr<IService>(IServiceProvider*)>;

struct ServiceDescriptor
{
    ServiceType type;
    Factory factory;
};

class ServiceRoot : IServiceProvider
{
public:
    explicit ServiceRoot() {}

    std::unordered_map<size_t, ServiceDescriptor> registry;
    std::unordered_map<size_t, std::unique_ptr<IService>> singletons;

    template<typename TService, typename... TDeps>
    void addService(ServiceType tp) {
        static_assert(std::is_base_of_v<IService, TService>, "TService must derive from IService");
        //проверки:
        //TService = Singleton, TDeps = Singleton
        //TService = Scoped, TDeps = Scoped/Singleton
        Factory f = [](IServiceProvider* prov) {
            return std::make_unique<TService>(
                static_cast<TDeps*>(prov->getRaw(typeid(TDeps).hash_code()))...
                );
        };
        //в конструктор передадутся сырые указатели
        registry[typeid(TService).hash_code()] = { tp, f };
    }

    template<typename T>
    ServiceType getServiceType()
    {
        size_t key = typeid(T).hash_code();
        ServiceDescriptor& descriptor = registry.at(key);
        return descriptor.type;
    }

    ServiceType getServiceType(size_t key)
    {
        ServiceDescriptor& descriptor = registry.at(key);
        return descriptor.type;
    }

    IService* getRaw(size_t key) override {
        ServiceDescriptor& descriptor = registry.at(key);
        switch(descriptor.type) {
        case ServiceType::Singleton: //владение у this/root
            if (!singletons.count(key))
                singletons[key] = descriptor.factory(this);
            return singletons[key].get();

        case ServiceType::Scoped:
            throw std::logic_error("Scoped in root");

        case ServiceType::Transient: //владение у вызывающего
            return descriptor.factory(this).release();
        }
        return nullptr;
    }
};

class ServiceScope : IServiceProvider
{
public:
    explicit ServiceScope(ServiceRoot* root)
        : root(root) {}

    IService* getRaw(size_t key) override {
        ServiceDescriptor& descriptor = root->registry.at(key);

        switch(descriptor.type) {
        case ServiceType::Singleton: //владение у root
            return root->getRaw(key);

        case ServiceType::Scoped: //владение у this
            if (!scoped.count(key))
                scoped[key] = descriptor.factory(this);
            return scoped[key].get();

        case ServiceType::Transient: //владение у вызывающего
            return descriptor.factory(this).release();
        }
        return nullptr;
    }

    template<typename T>
    ServiceType getServiceType()
    {
        return root->getServiceType<T>();
    }

    ServiceType getServiceType(size_t key)
    {
        return root->getServiceType(key);
    }

private:
    ServiceRoot* root;
    std::unordered_map<size_t, std::unique_ptr<IService>> scoped;
};

#endif // SERVICECOLLECTION_H
*/
#endif // IENDPOINT_H
