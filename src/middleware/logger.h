#ifndef LOGGER_H
#define LOGGER_H
#include "imiddleware.h"

namespace Middlewares
{
class Logger : public IMiddleware
{
public:
    QString name() const override { return "Logger"; }
    void privateInvoke(MessageContext& ctx, const RequestDelegate& next) override
    {
        next(ctx);
    }
};
}

#endif // LOGGER_H
