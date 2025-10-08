#include <QCoreApplication>
#include <QTimer>
#include "core/applicationserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationName("NeobankServer");
    a.setOrganizationName("R.I.Inc.");
    a.setApplicationVersion("1.0");
    ApplicationServer server(&a);
    QTimer::singleShot(0, &server, &ApplicationServer::startServer);
    return a.exec();
}
