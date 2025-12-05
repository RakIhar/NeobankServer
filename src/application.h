#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include "network/transportlayer.h"
// #include "context/contextwrapper.h"
#include "pipeline/pipelinebuilder.h"

class Application : public QObject
{
    Q_OBJECT
public:
    explicit Application(QObject *parent = nullptr);
    void start();
    void stop();

    void build(RequestDelegate endpoint)//построить делегат, подключить сигналы/слоты
    {
        m_requestDelegate = m_pipelineBuilder.build(endpoint);
    }
    void useEndpoint(QString name, QSharedPointer<IEndpoint> e)//тоже синглтон, потому что мне лень писать крутой код
    {
        m_endpoints.registerEndpoint(name, e);//заменить на std_shared_ptr, обдумать указатели в контексте
    }
    void useMiddleware(std::unique_ptr<IMiddleware> m)//синглтон, поэтому без возни с шаблонами
    {
        m_pipelineBuilder.addMiddleware(std::move(m));
    }
    template<typename TService, typename TImpl>
    void useService(ServiceType type)
    {
        m_serviceProvider.addService<TService, TImpl>(type);
    }
    template<typename TService>
    void useService(ServiceType type)
    {
        m_serviceProvider.addService<TService>(type);
    }
private:
    //host->wrapper->delegate->wrapper->delegate
    TransportLayer m_host;
    RequestDelegate m_requestDelegate; //результат работы билдера конвейера
    PipeLineBuilder m_pipelineBuilder; //middleware хранятся здесь
    EndpointRegistry m_endpoints; //передаются в contextWrapper
    ServiceProvider m_serviceProvider; //это root, который передается в конструктор ServiceScoped, который передаётся в contextWrapper
signals:
};

#endif // APPLICATION_H
