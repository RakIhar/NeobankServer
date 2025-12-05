#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QObject>
#include "imiddleware.h"

class Authorization : public QObject, public IMiddleware
{
    Q_OBJECT
public:
    explicit Authorization(QObject *parent = nullptr);

signals:
};

#endif // AUTHORIZATION_H
