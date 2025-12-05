#ifndef ENDPOINTINVOKER_H
#define ENDPOINTINVOKER_H

#include <QObject>
#include "imiddleware.h"
/*
void invoke(MessageContext& ctx, const RequestDelegate& next) override
{
    auto ep = ctx.items["endpoint"].value<const EndpointDelegate*>();
    if (ep) {
        (*ep)(ctx);  // вызвать endpoint
        return;
    }

    next(ctx); // если роута нет
}
*/
class EndpointInvoker : public QObject, public IMiddleware
{
    Q_OBJECT
public:
    explicit EndpointInvoker(QObject *parent = nullptr);

signals:
};

/*
[Pipeline]
    ↓
Middleware chain
    ↓
Routing
    ↓
EndpointInvokerMiddleware
    ↓
Endpoint handler (тонкий)
    ↓
DI container resolve
    ↓
Бизнес-логика (сервисы)

*/

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

#endif // ENDPOINTINVOKER_H
