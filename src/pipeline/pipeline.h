#ifndef PIPELINE_H
#define PIPELINE_H

#include <QObject>
#include "middleware.h"

class PipeLine : public QObject
{
    Q_OBJECT
public:
    explicit PipeLine(QObject *parent = nullptr);

    void use(MiddlewareFactory factory)
    {
        m_factories.push_back(std::move(factory));
    };

    RequestDelegate build(RequestDelegate endpoint)
    {
        RequestDelegate next = std::move(endpoint);

        for (int i = m_factories.size() - 1; i >= 0; --i)
        {
            MiddlewareFactory& factory = m_factories[i];

            RequestDelegate current = next;
            next = [factory, current](MessageContext& ctx){
                std::unique_ptr<IMiddleware> instance = factory();
                instance->invoke(ctx, current);
            };
        }

        return handler = next;
    };
    RequestDelegate handler = nullptr;
private:
    QVector<MiddlewareFactory> m_factories;

public slots:
    void onRequest(MessageContext& ctx)
    {
        handler(ctx);
        emit answer(ctx);
    }
signals:
    void answer(MessageContext& ctx );
};

/*
C#
public class TestMiddleware
{
    private readonly RequestDelegate next;

    public TestMiddleware(RequestDelegate next[, ...])
    {
        this.next = next;
    }

    public async Task InvokeAsync(HttpContext context)
    {
        var token = context.Request.Query["token"];
        if (token!="12345678")
        {
            context.Response.StatusCode = 403;
            await context.Response.WriteAsync("Token is invalid");
        }
        else
        {
            await next.Invoke(context);
        }
    }
}
*/

#endif // PIPELINE_H
