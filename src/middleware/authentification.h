#ifndef MIDDLEWARE_AUTHENTIFICATION_H
#define MIDDLEWARE_AUTHENTIFICATION_H
#include "imiddleware.h"
#include <QJsonObject>
#include <QUuid>

namespace Middlewares
{
class Authentification : public IMiddleware
{
    void authenticate(MessageContext& ctx);
public:
    QString name() const override { return "Authentification"; }

    void privateInvoke(MessageContext& ctx, const RequestDelegate& next) override
    {
        authenticate(ctx);
        next(ctx);
        //Нельзя прерывать, не все mdlware/endpts требуют аутентификации
    }
};
}

#endif // AUTHENTIFICATION_H

