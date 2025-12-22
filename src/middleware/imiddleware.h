#ifndef IMIDDLEWARE_H
#define IMIDDLEWARE_H
#include <functional>
#include "../context/messagecontext.h"

using RequestDelegate = std::function<void(MessageContext&)>;

class IMiddleware
{
    virtual void privateInvoke(MessageContext& ctx, const RequestDelegate& next) = 0;
public:
    virtual ~IMiddleware() = default;

    virtual QString name() const = 0;

    void invoke(MessageContext& ctx, const RequestDelegate& next)
    {
        if (!ctx.isAborted)
        {
            try
            {
                qDebug() << QString("[%1 middleware] enter").arg(name());
                privateInvoke(ctx, next);
                qDebug() << QString("[%1 middleware] exit").arg(name());
            }
            catch (...)
            {
                qDebug() << QString("[%1 middleware] abort").arg(name());
                ctx.abort();
            }
        }
    };
};

#endif // IMIDDLEWARE_H
