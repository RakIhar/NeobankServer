#ifndef SERVICECOLLECTION_H
#define SERVICECOLLECTION_H

#include <functional>
#include <memory>
#include <QDebug>
enum class ServiceType
{
    Transient,
    Scoped,
    Singleton
};

class IService
{
public:
    IService() = default; //наличие конструктора без параметров у наследников - ассерты
    //к сожалению концепты и requires только с 20-х крестов
    virtual ~IService() = default;
};

using Factory = std::function<std::unique_ptr<IService>()>;

struct ServiceDescriptor
{
    ServiceType type;
    Factory factory;
};

class ServiceProvider
{
public:
    explicit ServiceProvider() {}

    template<typename TService>
    void addService(ServiceType type)
    {
        static_assert(std::is_base_of_v<IService, TService>, "TService must derive from IService");
        static_assert(std::is_default_constructible_v<TService>, "Service must have default constructor");

        Factory f = []{
            // return std::unique_ptr<IService>(static_cast<IService*>(new TImpl()));
            return std::make_unique<TService>();
        };
        qDebug() << typeid(TService).hash_code();
        registry[typeid(TService).hash_code()] = ServiceDescriptor{ type, f };
    }

    template<typename TService, typename TImpl>
    void addService(ServiceType type)
    {
        static_assert(std::is_base_of_v<TService, TImpl>, "TImpl must derive from TService");
        static_assert(std::is_base_of_v<IService, TService>, "TService must derive from IService");
        static_assert(std::is_default_constructible_v<TImpl>, "Service must have default constructor");

        Factory f = []{
            // return std::unique_ptr<IService>(static_cast<IService*>(new TImpl()));
            return std::make_unique<TImpl>();
        };
        //RVO: компилятор создает unique_ptr в месте назначение, без копирования
        //или move если не rvo, крч кресты пиздец всратое говно
        registry[typeid(TService).hash_code()] = ServiceDescriptor{ type, f };
    }

    template<typename T>
    T* getSingleton();

    template<typename T>
    ServiceType getServiceType();

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
    ServiceType getServiceType();

    template<typename T>
    std::unique_ptr<T> getTransient();

private:
    ServiceProvider* root;
    std::unordered_map<size_t, std::unique_ptr<IService>> scoped;
};

#endif // SERVICECOLLECTION_H
