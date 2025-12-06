#ifndef ROUTER_H
#define ROUTER_H

#include <QObject>
#include "imiddleware.h"

class Router : public QObject, public IMiddleware
{
    Q_OBJECT
public:
    explicit Router(QObject *parent = nullptr);

signals:
};

#endif // ROUTER_H
