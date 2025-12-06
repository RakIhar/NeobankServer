#include "application.h"
#include "context/contextwrapper.h"

Application::Application(QObject *parent) : QObject{parent}
{
    connect(&m_host, &TransportLayer::messageReceived, this,
            [this](const QUuid connection, const QByteArray rawData){
                std::optional<MessageContext> ctxOpt = ContextWrapper::createContext(
                    connection, rawData, m_endpoints, ServiceScope(m_serviceProvider));
                if (!ctxOpt)
                    return;
                MessageContext ctx = std::move(*ctxOpt);
                m_requestDelegate(ctx); //передача по ссылке. Время жизни контекста - эта область видимости
                std::pair<QUuid, QByteArray> reply = ContextWrapper::reduceContext(ctx);
                m_host.sendMessage(reply.first, reply.second);
            });

    m_serviceProvider = new ServiceProvider();
}
