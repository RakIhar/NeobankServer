#ifndef ENDPOINTINVOKER_H
#define ENDPOINTINVOKER_H

#include <QObject>
#include "imiddleware.h"

namespace Middlewares{

class EndpointInvoker : public IMiddleware
{
public:
    void invoke(MessageContext& ctx, const RequestDelegate& next) override {
        if (!ctx.isAborted)
        {
            try
            {
                qDebug() << "[EndpointInvoker] enter";
                if (ctx.currentEndpoint != nullptr)
                {
                    ctx.currentEndpoint->invoke(ctx);
                }
                else
                {
                    qDebug() << "[EndpointInvoker] no route";
                }

                next(ctx);
                qDebug() << "[EndpointInvoker] exit";
            }
            catch (...)
            {
                qDebug() << "[EndpointInvoker] abort";
                ctx.abort();
            }
        }
    }
};

}

#endif // ENDPOINTINVOKER_H
