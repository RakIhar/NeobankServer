#ifndef ROUTER_H
#define ROUTER_H

#include "imiddleware.h"

namespace Middlewares{

class Router : public IMiddleware
{
public:
    void invoke(MessageContext& ctx, const RequestDelegate& next) override {
        if (!ctx.isAborted)
        {
            try
            {
                qDebug() << "[Router] enter";

                QString route = ctx.jsonRequest["endpoint"].toString();

                ctx.currentEndpoint = ctx.endpoints.getEndpoint(route);

                next(ctx);

                qDebug() << "[Router] exit";
            }
            catch (...)
            {
                qDebug() << "[Router] abort";
                ctx.abort();
            }
        }
    }
};

}

#endif // ROUTER_H
