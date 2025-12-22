#ifndef EXCEPTION_H
#define EXCEPTION_H
#include "imiddleware.h"

namespace Middlewares
{
class Exception : public IMiddleware
{
public:
    QString name() const override { return "Exception"; }
    void privateInvoke(MessageContext& ctx, const RequestDelegate& next) override
    {
        next(ctx);
    }
};
}
#endif // EXCEPTION_H
