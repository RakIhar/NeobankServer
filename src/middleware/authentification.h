#ifndef MIDDLEWARE_AUTHENTIFICATION_H
#define MIDDLEWARE_AUTHENTIFICATION_H

#include "imiddleware.h"
#include <QJsonObject>
#include <QUuid>

namespace Middlewares{

class Authentification : public IMiddleware
{
    void authenticate(MessageContext& ctx);
public:
    void invoke(MessageContext& ctx, const RequestDelegate& next) override {
        if (!ctx.isAborted)
        {
            try
            {
                qDebug() << "[Authentification] enter";
                
                authenticate(ctx);
                
                next(ctx); //Нельзя прерывать, не все mdlware/endpts требуют аутентификации
                qDebug() << "[Authentification] exit";
            }
            catch (...)
            {
                qDebug() << "[Authentification] abort";
                ctx.abort();
            }
        }
    }
};

}

#endif // AUTHENTIFICATION_H

