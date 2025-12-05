#ifndef SERVICECOLLECTION_H
#define SERVICECOLLECTION_H

#include <functional>
#include <memory>

enum class ServiceType
{
    Transient,
    Scoped,
    Singleton
};

class IService
{
public:
    IService() = default; //как обеспечить наличие конструктора без параметров у наследников? - через ассерты бляяяя
    //к сожалению концепты и requires только с 20-х крестов
    virtual ~IService() = default;
};

using Factory = std::function<std::unique_ptr<IService>()>;

struct ServiceDescriptor
{
    ServiceType type;
    Factory factory;
};

class ServiceCollection
{
public:
    template<typename TService, typename TImpl>
    void addService(ServiceType type);

    template<typename TService>
    void addService(ServiceType type);

    std::unordered_map<size_t, ServiceDescriptor> services;
};

class ServiceProvider
{
public:
    explicit ServiceProvider(std::unordered_map<size_t, ServiceDescriptor> registry)
        : registry(std::move(registry)) {}

    template<typename T>
    T* getSingleton();

    std::unordered_map<size_t, ServiceDescriptor> registry;
    std::unordered_map<size_t, std::unique_ptr<IService>> singletons;
};

class ServiceScope
{
public:
    explicit ServiceScope(ServiceProvider* root)
        : root(root) {}

    template<typename T>
    T* get();

    template<typename T>
    std::unique_ptr<T> getTransient();

private:
    ServiceProvider* root;
    std::unordered_map<size_t, std::unique_ptr<IService>> scoped;
};

#endif // SERVICECOLLECTION_H
