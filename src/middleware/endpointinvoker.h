#ifndef ENDPOINTINVOKER_H
#define ENDPOINTINVOKER_H

#include <QObject>
#include "imiddleware.h"

class EndpointInvoker : public QObject, public IMiddleware
{
    Q_OBJECT
public:
    explicit EndpointInvoker(QObject *parent = nullptr);

signals:
};

#endif // ENDPOINTINVOKER_H
