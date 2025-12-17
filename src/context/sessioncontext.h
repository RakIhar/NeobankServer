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
    QUuid session_id;
    QString token;
    QHash<QVariant, QVariant> items;
};

}

#endif // SESSIONCONTEXT_H
