#ifndef LOGGER_H
#define LOGGER_H

#include "imiddleware.h"
#include "../service/iservice.h"

namespace Middlewares{

class Logger : public IMiddleware
{
public:
    void invoke(MessageContext& ctx, const RequestDelegate& next) override {
        if (!ctx.isAborted)
        {
            try
            {
                qDebug() << "[Logger] enter";
                next(ctx);
                qDebug() << "[Logger] exit";
            }
            catch (...)
            {
                qDebug() << "[Logger] abort";
                ctx.abort();
            }
        }
    }
};

}

#endif // LOGGER_H
