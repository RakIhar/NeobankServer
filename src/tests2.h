#ifndef TESTS2_H
#define TESTS2_H

// #include "service/iservice.h"       //ISERVICE
// #include "middleware/imiddleware.h" //IMIDDLEWARE
// #include "context/context.h"        //IENDPOINT

/*
#include "tests2.h"

app.useService<CounterServiceTransient>(ServiceType::Transient);
app.useService<CounterServiceScoped>(ServiceType::Scoped);
app.useService<CounterServiceSingleton>(ServiceType::Singleton);

app.useMiddleware(std::make_unique<Middleware1>());
app.useMiddleware(std::make_unique<Middleware2>());
app.useMiddleware(std::make_unique<Middleware3>());
app.useMiddleware(std::make_unique<Middleware4>());

app.useEndpoint("endpoint1", std::make_unique<Endpoint1>());
app.useEndpoint("endpoint2", std::make_unique<Endpoint2>());
*/

/*class CounterServiceTransient : public IService {
private:
    int total = 0;
public:
    int get() { return ++total; }
    ~CounterServiceTransient() { qDebug() << "~CounterServiceTransient()"; }
};

class CounterServiceScoped : public IService {
private:
    int total = 0;
public:
    int get() { return ++total; }
    ~CounterServiceScoped() { qDebug() << "~CounterServiceScoped()"; }
};

class CounterServiceSingleton : public IService {
private:
    int total = 0;
public:
    int get() { return ++total; }
    ~CounterServiceSingleton() { qDebug() << "~CounterServiceSingleton()"; }
};

class Middleware1 : public IMiddleware {
public:
    void invoke(MessageContext& ctx, const RequestDelegate& next) override {
        qDebug() << "[Middleware1] enter";

        std::unique_ptr<CounterServiceTransient> c1 = ctx.services.getTransient<CounterServiceTransient>();
        CounterServiceScoped* c2 = ctx.services.get<CounterServiceScoped>();
        CounterServiceSingleton* c3 = ctx.services.get<CounterServiceSingleton>();
        qDebug() << "Transient" << c1.get()->get();
        qDebug() << "Scoped" << c2->get();
        qDebug() << "Singleton" << c3->get();

        next(ctx);

        qDebug() << "[Middleware1] exit";
    }
};

class Middleware2 : public IMiddleware {
private:
    int count = 0;
public:
    void invoke(MessageContext& ctx, const RequestDelegate& next) override {
        qDebug() << "[CounterMiddleware] enter"
                 << "Req count : " << ++count;

        std::unique_ptr<CounterServiceTransient> c1 = ctx.services.getTransient<CounterServiceTransient>();
        CounterServiceScoped* c2 = ctx.services.get<CounterServiceScoped>();
        CounterServiceSingleton* c3 = ctx.services.get<CounterServiceSingleton>();
        qDebug() << "Transient" << c1.get()->get();
        qDebug() << "Scoped" << c2->get();
        qDebug() << "Singleton" << c3->get();

        next(ctx);

        qDebug() << "[CounterMiddleware] exit";
    }
};

class Middleware3 : public IMiddleware {
public:
    void invoke(MessageContext& ctx, const RequestDelegate& next) override {
        ctx.jsonResponce["state"] = "processed";
        qDebug() << "[Middleware3] enter. State set to 'processed'";
        CounterServiceSingleton* c = ctx.services.get<CounterServiceSingleton>();
        int x = c->get();
        qDebug() << "Singleton" << x;
        if (x % 2 == 0)
        {
            qDebug() << "[Middleware3] selected endpoint 1";
            ctx.currentEndpoint = ctx.endpoints.getEndpoint("endpoint1");
        }
        else
        {
            qDebug() << "[Middleware3] selected endpoint 2";
            ctx.currentEndpoint = ctx.endpoints.getEndpoint("endpoint2");
        }

        next(ctx);

        qDebug() << "[Middleware3] exit";
    }
};

class Middleware4 : public IMiddleware {
public:
    void invoke(MessageContext& ctx, const RequestDelegate& next) override {
        ctx.jsonResponce["state"] = "processed";
        qDebug() << "[StateMiddleware] State set to 'processed'";
        ctx.currentEndpoint->invoke(ctx);
        next(ctx);
        qDebug() << "[StateMiddleware] exit";
    }
};

class Endpoint1 : public IEndpoint {
public:
    void invoke(MessageContext& ctx) override {
        qDebug() << "[endpoint1] invoked";
    }
};

class Endpoint2 : public IEndpoint {
public:
    void invoke(MessageContext& ctx) override {
        qDebug() << "[endpoint2] invoked";
    }
};*/

#endif // TESTS2_H
