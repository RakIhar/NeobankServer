#ifndef AUTHENTIFICATION_H
#define AUTHENTIFICATION_H

#include <QObject>
#include "imiddleware.h"
#include "../service/service.h"

class AuthentificationMiddleware : public QObject, public IMiddleware
{
    Q_OBJECT
public:
    explicit AuthentificationMiddleware(QObject *parent = nullptr) {}

signals:
};

class AuthentificationService : public QObject, public IService
{
    Q_OBJECT
public:
    explicit AuthentificationService(QObject *parent = nullptr) {}

signals:
};

#endif // AUTHENTIFICATION_H
