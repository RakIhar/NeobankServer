#include <QCoreApplication>
#include <QTimer>
#include "network/transportlayer.h"
#include "context/contextwrapper.h"
#include "pipeline/pipeline.h"
#include <QObject>
#include "middleware/authentification.h"
#include "middleware/authorization.h"
// #include "middleware/router.h"
#include "middleware/logger.h"
#include "middleware/session.h"
#include "middleware/endpointinvoker.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationName("NeobankServer");
    a.setOrganizationName("R.I.Inc.");
    a.setApplicationVersion("1.0");

    TransportLayer host;
    ContextWrapper wrapper;

    QObject::connect(&host, &TransportLayer::messageReceived,
                     &wrapper, &ContextWrapper::onCreateContext);

    QObject::connect(&wrapper, &ContextWrapper::contextReduced,
                     &host, &TransportLayer::onSendMessage);

    PipeLine pipeline;
    pipeline.use([]{ return std::make_unique<Authentification>(); });
    pipeline.use([]{ return std::make_unique<Authorization>(); });
    // pipeline.use([&]{ return std::make_unique<Router>(&endpoints); });
    pipeline.use([]{ return std::make_unique<Logger>(); });
    pipeline.use([]{ return std::make_unique<Session>(); });
    pipeline.use([]{ return std::make_unique<EndpointInvoker>(); });
    RequestDelegate terminal = [](MessageContext& ctx) {
        qDebug() << "Not found";
    };
    //тут добавление сервисов
    pipeline.build(terminal);

    QObject::connect(&wrapper, &ContextWrapper::contextCreated,
                     &pipeline, &PipeLine::onRequest);
    QObject::connect(&pipeline, &PipeLine::answer,
                     &wrapper, &ContextWrapper::onReduceContext);

    QTimer::singleShot(0, &host, &TransportLayer::start);
    return a.exec();
}
