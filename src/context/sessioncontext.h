#ifndef SESSIONCONTEXT_H
#define SESSIONCONTEXT_H
#include <QHash>
#include <QUuid>
#include <QVariant>

namespace Context {

class Session
{
public:
    Session() {};
    bool isAvailable;
    QUuid sessionId;
    QHash<QVariant, QVariant> items;
};

}

#endif // SESSIONCONTEXT_H
