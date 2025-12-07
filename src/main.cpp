#include <QCoreApplication>
#include <QTimer>
#include <QObject>

#include "application.h"

#include "middleware/logger.h"
#include "middleware/exception.h"
#include "middleware/session.h"
#include "middleware/authentification.h"
#include "middleware/router.h"
#include "middleware/authorization.h"
#include "middleware/endpointinvoker.h"

#include "service/authentification.h"
#include "database/dbmanager.h"
#include "database/repositories/accountrepository.h"
#include "database/repositories/auditlogrepository.h"
#include "database/repositories/authsessionrepository.h"
#include "database/repositories/transactionrepository.h"
#include "database/repositories/userrepository.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationName("NeobankServer");
    a.setOrganizationName("R.I.Inc.");
    a.setApplicationVersion("1.0");

    Application app;

    {
    using namespace Database;
    app.useService<DataBaseManager                         >(ServiceType::Singleton);
    app.useService<AccountRepository,       DataBaseManager>(ServiceType::Singleton);
    app.useService<AuditLogRepository,      DataBaseManager>(ServiceType::Singleton);
    app.useService<AuthSessionRepository,   DataBaseManager>(ServiceType::Singleton);
    app.useService<TransactionRepository,   DataBaseManager>(ServiceType::Singleton);
    app.useService<UserRepository,          DataBaseManager>(ServiceType::Singleton);
    }
    {
    using namespace Services;
    app.useService<Authentification, Database::AuthSessionRepository>(ServiceType::Singleton);
    }
    app.useMiddleware(std::make_unique<Middlewares::Logger>());
    app.useMiddleware(std::make_unique<Middlewares::Exception>());
    app.useMiddleware(std::make_unique<Middlewares::Session>());          // извлекает sessionId
    app.useMiddleware(std::make_unique<Middlewares::Authentification>()); // login / validateSession //встраивается до любых middleware, требующий аутентификацию
    app.useMiddleware(std::make_unique<Middlewares::Router>());           // ctx.currentEndpoint = ...
    app.useMiddleware(std::make_unique<Middlewares::Authorization>());    // проверка прав
    app.useMiddleware(std::make_unique<Middlewares::EndpointInvoker>());

    RequestDelegate terminal = [](MessageContext& ctx) {
        qDebug() << "Hello, world!";
    };
    app.build(terminal);
    QTimer::singleShot(0, &app, &Application::start);
    return a.exec();
}

