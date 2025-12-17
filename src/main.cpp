#include <QCoreApplication>
#include <QTimer>
#include <QObject>

#include "application.h"
#include "common/constants.h"

#include "middleware/logger.h"
#include "middleware/exception.h"
#include "middleware/session.h"
#include "middleware/authentification.h"
#include "middleware/router.h"
#include "middleware/authorization.h"
#include "middleware/endpointinvoker.h"

#include "database/dbmanager.h"
#include "database/repositories/accountrepository.h"
#include "database/repositories/auditlogrepository.h"
#include "database/repositories/authsessionrepository.h"
#include "database/repositories/transactionrepository.h"
#include "database/repositories/userrepository.h"

#include "service/session.h"
#include "service/authentification.h"

#include "endpoint/login.h"
#include "endpoint/register.h"
#include "endpoint/accountlist.h"
#include "endpoint/accountcreate.h"
#include "endpoint/transactionlist.h"
#include "endpoint/transactioncreate.h"

int main(int argc, char *argv[])
{
    //TODO разграничить auth_session и logic_session
    //TODO сделать многопоточность
    //TODO дописать сервисы и эндпоинты, связанные конкретно с логикой банка
    //TODO убрать различные REMOVE
    //TODO в сервисах добавить nullptr
    //TODO обдумать QVariant в TransactionCreate
    //TODO обдумать переводы валют в TransactionCreate
    QCoreApplication a(argc, argv);
    a.setApplicationName("NeobankServer");
    a.setOrganizationName("R.I.Inc.");
    a.setApplicationVersion("1.0");

    Application app;

    {
    using namespace Database;
    app.useService<DataBaseManager>(ServiceType::Singleton);
    app.useService<AccountRepository,       DataBaseManager>(ServiceType::Singleton);
    app.useService<AuditLogRepository,      DataBaseManager>(ServiceType::Singleton);
    app.useService<AuthSessionRepository,   DataBaseManager>(ServiceType::Singleton);
    app.useService<TransactionRepository,   DataBaseManager>(ServiceType::Singleton);
    app.useService<UserRepository,          DataBaseManager>(ServiceType::Singleton);

    using namespace Services;
    app.useService<Session,          AuthSessionRepository>(ServiceType::Singleton);
    app.useService<Authentification, AuthSessionRepository, UserRepository>(ServiceType::Singleton);
    }
    {
    using namespace Middlewares;
    app.useMiddleware(std::make_unique<Logger>());
    app.useMiddleware(std::make_unique<Exception>());
    app.useMiddleware(std::make_unique<Authentification>());
    app.useMiddleware(std::make_unique<Session>());
    app.useMiddleware(std::make_unique<Router>());
    app.useMiddleware(std::make_unique<Authorization>());
    app.useMiddleware(std::make_unique<EndpointInvoker>());
    }
    {
    using namespace Endpoints;
    using namespace Common;
    app.useEndpoint(toStr(ProtocolType::Login),     std::make_unique<Login>());
    app.useEndpoint(toStr(ProtocolType::Register),  std::make_unique<Register>());
    app.useEndpoint(toStr(ProtocolType::AccList),   std::make_unique<AccountList>());
    app.useEndpoint(toStr(ProtocolType::TrList),    std::make_unique<TransactionList>());
    app.useEndpoint(toStr(ProtocolType::AccCreate), std::make_unique<AccountCreate>());
    app.useEndpoint(toStr(ProtocolType::TrCreate),  std::make_unique<TransactionCreate>());
    }
    RequestDelegate terminal = [](MessageContext& ctx) {
        qDebug() << "Hello, world!";
    };
    app.build(terminal);
    QTimer::singleShot(0, &app, &Application::start);
    return a.exec();
}
