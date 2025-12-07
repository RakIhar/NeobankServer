#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include "imiddleware.h"
#include "../service/iservice.h"

namespace Middlewares{

class Authorization : public IMiddleware
{
public:
    void invoke(MessageContext& ctx, const RequestDelegate& next) override {
        if (!ctx.isAborted)
        {
            try
            {
                qDebug() << "[Authorization] enter";
                //логика, вызов сервисов
                next(ctx);
                qDebug() << "[Authorization] exit";
            }
            catch (...)
            {
                qDebug() << "[Authorization] abort";
                ctx.abort();
            }
        }
    }
};

}

#endif // AUTHORIZATION_H
