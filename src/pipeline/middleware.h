#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H
#include <functional>
#include "../context/messagecontext.h"

using RequestDelegate = std::function<void(MessageContext&)>;

class IMiddleware {
public:
    virtual ~IMiddleware() = default;

    virtual void invoke(MessageContext& ctx, const RequestDelegate& next) = 0;
};

using MiddlewareFactory = std::function<std::unique_ptr<IMiddleware>()>;

/*
class RouterMiddleware : public IMiddleware {
public:
    RouterMiddleware(const EndpointCollection* endpoints)
        : m_endpoints(endpoints) {}

    void invoke(RequestContext& ctx, const RequestDelegate& next) override {
        QString path = ctx.items["path"].toString();

        const Endpoint* ep = m_endpoints->match(path);
        if (ep) {
            ctx.items["endpoint"] = QVariant::fromValue((void*)ep);
        }

        next(ctx); // всегда продолжаем
    }

private:
    const EndpointCollection* m_endpoints;
};

class EndpointInvokerMiddleware : public IMiddleware {
public:
    void invoke(RequestContext& ctx, const RequestDelegate& next) override {
        auto epPtr = ctx.items["endpoint"].value<void*>();

        if (epPtr == nullptr) {
            next(ctx); // endpoint не найден, fallback
            return;
        }

        const Endpoint* ep = static_cast<const Endpoint*>(epPtr);

        ep->handler(ctx); // вот здесь вызываем endpoint

        // Важно: после endpoint НЕТ next
    }
};

PipelineBuilder builder;
builder.use([]{ return std::make_unique<LoggingMiddleware>(); });
builder.use([]{ return std::make_unique<CorsMiddleware>(); });
builder.use([&]{ return std::make_unique<RouterMiddleware>(&endpoints); });
builder.use([]{ return std::make_unique<AuthMiddleware>(); });
builder.use([]{ return std::make_unique<AuthZMiddleware>(); });
builder.use([]{ return std::make_unique<EndpointInvokerMiddleware>(); });
// fallback endpoint
RequestDelegate terminal = [](RequestContext& ctx) {
    ctx.response = "404 Not Found";
};
RequestDelegate pipeline = builder.build(terminal);
*/

#endif // MIDDLEWARE_H
