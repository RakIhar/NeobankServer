#include <QCoreApplication>
#include <QTimer>
#include "network/transportlayer.h"
#include "context/contextwrapper.h"
#include <QObject>

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

    QTimer::singleShot(0, &host, &TransportLayer::start);
    return a.exec();
}
