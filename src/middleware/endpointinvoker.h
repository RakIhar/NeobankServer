#ifndef ENDPOINTINVOKER_H
#define ENDPOINTINVOKER_H

#include <QObject>
#include "../pipeline/middleware.h"
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

#endif // ENDPOINTINVOKER_H
