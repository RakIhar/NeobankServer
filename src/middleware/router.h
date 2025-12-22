#ifndef ROUTER_H
#define ROUTER_H
#include "imiddleware.h"
#include "../common/constants.h"
#include "../endpoint/iendpoint.h"

namespace Middlewares
{
class Router : public IMiddleware
{
public:
    QString name() const override { return "Router"; }

    void privateInvoke(MessageContext& ctx, const RequestDelegate& next) override
    {
        using namespace Common;
        QString route = ctx.jsonRequest[toStr(JsonField::Type)].toString();
        ctx.currentEndpoint = ctx.endpoints.getEndpoint(route);
        next(ctx);
    }
};
}

#endif // ROUTER_H
