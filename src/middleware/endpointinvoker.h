#ifndef ENDPOINTINVOKER_H
#define ENDPOINTINVOKER_H
#include <QObject>
#include "imiddleware.h"
#include "../endpoint/iendpoint.h"

namespace Middlewares
{
class EndpointInvoker : public IMiddleware
{
public:
    QString name() const override { return "EndpointInvoker"; }

    void privateInvoke(MessageContext& ctx, const RequestDelegate& next) override
    {
        if (ctx.currentEndpoint != nullptr)
        {
            ctx.currentEndpoint->invoke(ctx);
        }
        else
        {
            qDebug() << QString("[%1] no route").arg(name());
        }
        next(ctx);
    }
};
}

#endif // ENDPOINTINVOKER_H
