#ifndef CONNECTIONCONTEXT_H
#define CONNECTIONCONTEXT_H
#include <QString>
#include <QUuid>

namespace Context
{
struct Connection
{
    QUuid id;
    QString ipAddress = "";
    QString userAgent = "";
};
}
#endif // CONNECTIONCONTEXT_H
