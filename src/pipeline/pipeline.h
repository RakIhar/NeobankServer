#ifndef PIPELINE_H
#define PIPELINE_H

#include <QObject>

class PipeLine : public QObject
{
    Q_OBJECT
public:
    explicit PipeLine(QObject *parent = nullptr);

signals:
};

//kestrel
//middleware(глобальные)
//filter before(1,2,3,..)(для конкретного endpoint)
//controller execution
//filter after(1,2,3,..)
//middleware
//kestrel

//Моя архитектура
//Transport layer: (Quuid, QByteArry) <- и ->
//ContextWrapper: (QUuid, QByteArray) <- и -> (MessageContext)
//Pipeline: exception, routing(выбор обработчиков), authentification, authorization, session, cache, log, endpoints(вызов обработчиков)
//Controllers сами обработчики
// next(context)

//IApplicationBuilder Use(Func<RequestDelegate, RequestDelegate> middleware);
//IApplicationBuilder UseMiddleware<TMiddleware>();

/*
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

public static class TokenExtensions
{
    public static IApplicationBuilder UseToken(this IApplicationBuilder builder[, ...])
    {
        return builder.UseMiddleware<TokenMiddleware>([...]);
    }
}
*/

/*
 Конвейер — это цепочка RequestDelegate (делегатов Task RequestDelegate(HttpContext)), собранная при старте приложения.
На каждый запрос вызывается корневой RequestDelegate, который последовательно вызывает следующий элемент цепочки.
Middleware инстанцируются один раз при старте (обычно) и получают HttpContext на каждое обращение.


Use(Func<HttpContext, Func<Task>, Task>) — добавляет промежуточный обработчик, который получает next и обязан вызвать его, если нужно.
Run(Func<HttpContext, Task>) — добавляет терминальный обработчик; фабрика не вызывает next.
Map(path, branchApp) — создаёт ветвь: если context.Request.Path.StartsWithSegments(path) — выполняется собранный делегат ветви,
    иначе — основной pipeline. Технически Map добавляет фабрику, которая проверяет путь и вызывает либо branchDelegate, либо next.
UseWhen(predicate, branch => { ... }) — похож на MapWhen, подставляет ветвь при условии predicate(context).

public delegate System.Threading.Tasks.Task RequestDelegate(HttpContext context);
void RequestDelegate(MessageContext);
*/


#endif // PIPELINE_H
