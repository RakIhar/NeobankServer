#include "service.h"

template<typename T>
std::unique_ptr<T> ServiceScope::getTransient() //так как нельзя Transient
{
    size_t key = typeid(T).hash_code();
    ServiceDescriptor& descriptor = root->registry.at(key);
    std::unique_ptr<IService> inst = descriptor.factory();
    return std::unique_ptr<T>( static_cast<T*>( inst.release() ) ); // передаём владение вызывающему
}

template<typename T>
T *ServiceScope::get()
{
    size_t key = typeid(T).hash_code();
    auto it = scoped.find(key);
    if (it != scoped.end())
        return static_cast<T*>(it->second.get());

    ServiceDescriptor& descriptor = root->registry.at(key);

    switch (descriptor.type) {
    case ServiceType::Singleton:
        return root->getSingleton<T>();

    case ServiceType::Scoped: {
        std::unique_ptr<IService> instance = descriptor.factory();
        T* raw = static_cast<T*>(instance.get());
        scoped[key] = std::move(instance);
        return raw;
    }

    case ServiceType::Transient:
        static_assert(!std::is_same_v<T, T>, "Use createTransient<T>() to obtain Transient services");
        //ПРИ ПОЛУЧЕНИИ НАДО БЫЛО ДЕЛАТЬ unique_ptr
        return static_cast<T*>(descriptor.factory().release());
    }
    return nullptr;
}

template<typename T>
T *ServiceProvider::getSingleton()
{
    size_t key = typeid(T).hash_code();
    auto it = singletons.find(key);
    if (it != singletons.end())
        return static_cast<T*>(it->second.get()); //сырой указатель из unique_ptr, приведение збс так как ассерты

    ServiceDescriptor& descriptor = registry.at(key);
    std::unique_ptr<IService> instance = descriptor.factory();
    T* raw = static_cast<T*>(instance.get());
    singletons[key] = std::move(instance);
    //перенос ибо после выхода из области видимости удалился бы
    return raw;
}

template<typename TService>
void ServiceProvider::addService(ServiceType type)
{
    static_assert(std::is_base_of_v<IService, TService>, "TService must derive from IService");
    static_assert(std::is_default_constructible_v<TService>, "Service must have default constructor");

    Factory f = []{
        // return std::unique_ptr<IService>(static_cast<IService*>(new TImpl()));
        return std::make_unique<TService>();
    };
    registry[typeid(TService).hash_code()] = ServiceDescriptor{ type, f };
}

template<typename TService, typename TImpl>
void ServiceProvider::addService(ServiceType type)
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
ServiceType ServiceScope::getServiceType()
{
    return root->getServiceType<T>();
}

template<typename T>
ServiceType ServiceProvider::getServiceType()
{
    size_t key = typeid(T).hash_code();
    ServiceDescriptor& descriptor = registry.at(key);
    return descriptor.type;
}
