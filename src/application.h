#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include "network/transportlayer.h"
#include "context/contextwrapper.h"
#include "pipeline/pipelinebuilder.h"

class Application : public QObject
{
    Q_OBJECT
public:
    explicit Application(QObject *parent = nullptr);
    void start();
    void stop();

    void build();//построить делегат, настроить m_serviceProvider, подключить сигналы/слоты

    void useEndpoint(IEndpoint);//тоже синглтон, потому что мне лень писать крутой код
    void useMiddleware(IMiddleware);//синглтон, поэтому без возни с шаблонами
    template<typename TService, typename TImpl>
    void useService(ServiceType type);
    template<typename TService>
    void useService(ServiceType type);
private:
    TransportLayer m_host;
    ContextWrapper m_wrapper;
    PipeLineBuilder m_pipelineBuilder; //middleware хранятся здесь
    EndpointRegistry m_endpoints; //передадутся в contextWrapper
    RequestDelegate requestDelegate;

    ServiceCollection m_serviceCollection; //хранит сервисы
    ServiceProvider m_serviceProvider; //забирает сервисы у serviceCollection, это root, который передается в ServiceScoped
signals:
};

#endif // APPLICATION_H
