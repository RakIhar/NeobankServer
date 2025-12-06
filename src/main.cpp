#include <QCoreApplication>
#include <QTimer>
#include <QObject>
#include "middleware/authentification.h"
#include "middleware/authorization.h"
#include "middleware/router.h"
#include "middleware/logger.h"
#include "middleware/session.h"
#include "middleware/endpointinvoker.h"
#include "application.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationName("NeobankServer");
    a.setOrganizationName("R.I.Inc.");
    a.setApplicationVersion("1.0");

    Application app;
    app.useService<AuthentificationService>(ServiceType::Singleton);
    app.useMiddleware(std::make_unique<EndpointInvoker>());
    RequestDelegate terminal = [](MessageContext& ctx) {
        qDebug() << "Hello, world!";
    };
    app.build(terminal);
    QTimer::singleShot(0, &app, &Application::start);
    return a.exec();
}
