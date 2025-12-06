#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include "network/transportlayer.h"
#include "pipeline/pipelinebuilder.h"

/*
Модель владения:
Application
 ├─ ServiceProvider (root, владелец)
 ├─ EndpointRegistry (root, владелец)
 └─ MessageContext  (на каждый запрос)
         ├─ ServiceScope (копия / value)
         └─ EndpointRegistry& (borrow)
*/

class Application : public QObject
{
    Q_OBJECT
public:
    explicit Application(QObject *parent = nullptr);
    void start() { m_host.start(); }
    void stop() { m_host.stop();}
    void build(RequestDelegate endpoint) { m_requestDelegate = m_pipelineBuilder.build(endpoint); }
    void useEndpoint(QString name, QSharedPointer<IEndpoint> e) { m_endpoints.registerEndpoint(name, e); }
    void useMiddleware(std::unique_ptr<IMiddleware> m) { m_pipelineBuilder.addMiddleware(std::move(m)); }
    template<typename TService, typename TImpl>
    void useService(ServiceType type) { m_serviceProvider->addService<TService, TImpl>(type); }
    template<typename TService>
    void useService(ServiceType type) { m_serviceProvider->addService<TService>(type); }
private:
    TransportLayer m_host;
    RequestDelegate m_requestDelegate;
    PipeLineBuilder m_pipelineBuilder;
    EndpointRegistry m_endpoints;
    ServiceProvider* m_serviceProvider;
signals:
};

#endif // APPLICATION_H
