#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "imiddleware.h"
#include "../service/iservice.h"

namespace Middlewares{

class Exception : public IMiddleware
{
public:
    void invoke(MessageContext& ctx, const RequestDelegate& next) override {
        if (!ctx.isAborted)
        {
            try
            {
                qDebug() << "[Exception] enter";
                next(ctx);
                qDebug() << "[Authorization] exit";
            }
            catch (...)
            {
                qDebug() << "[Exception] abort";
                ctx.abort();
            }
        }
    }
};

}
#endif // EXCEPTION_H
