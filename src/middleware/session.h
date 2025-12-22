#ifndef SESSION_H
#define SESSION_H
#include "imiddleware.h"

namespace Middlewares
{
class Session : public IMiddleware
{
public:
    QString name() const override { return "Session"; }

    void privateInvoke(MessageContext& ctx, const RequestDelegate& next) override
    {
        next(ctx);
    }
};
}

#endif // SESSION_H
