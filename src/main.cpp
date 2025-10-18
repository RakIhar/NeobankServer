#include <QCoreApplication>
#include <QTimer>
#include "network/ssl/sslservermanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationName("NeobankServer");
    a.setOrganizationName("R.I.Inc.");
    a.setApplicationVersion("1.0");
    SslServerManager server;
    QTimer::singleShot(0, &server, &SslServerManager::startServer);
    return a.exec();
}
