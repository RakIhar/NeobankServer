#ifndef TEST3_H
#define TEST3_H

#include "service/iservice.h"       //ISERVICE
#include "middleware/imiddleware.h" //IMIDDLEWARE
#include "context/context.h"        //IENDPOINT

/*
    // app.useService<DepA>(ServiceType::Singleton);
    // app.useService<DepB>(ServiceType::Scoped);
    // app.useService<DepC>(ServiceType::Transient);
    // app.useService<ComplexService, DepA, DepB, DepC>(ServiceType::Transient);
    // app.useMiddleware(std::make_unique<MiddlewareInjectTest>());
*/

class DepA : public IService {
public:
    int x = 10;
    DepA() { qDebug() << "DepA ctor"; }
    ~DepA() { qDebug() << "~DepA"; }
};

class DepB : public IService {
public:
    int y = 20;
    DepB() { qDebug() << "DepB ctor"; }
    ~DepB() { qDebug() << "~DepB"; }
};

class DepC : public IService { //transient, на него надо unique_ptr
public:
    int z = 20;
    DepC() { qDebug() << "DepC ctor"; }
    ~DepC() { qDebug() << "~DepC"; }
};


class ComplexService : public IService {
private:
    DepA* a;
    DepB* b;
    // DepC* c;
    std::unique_ptr<DepC> c;
public:
    ComplexService(DepA* a, DepB* b, DepC* c)
        : a(a), b(b), c(c)
    {
        qDebug() << "ComplexService ctor" << "a->x=" << a->x << "b->y=" << b->y << "c->z=" << c->z;
    }

    ~ComplexService() { qDebug() << "~ComplexService"; }

    int sum() const { return a->x + b->y + c->z; }
};

class MiddlewareInjectTest : public IMiddleware {
public:
    void invoke(MessageContext& ctx, const RequestDelegate& next) override
    {
        qDebug() << "[MiddlewareInjectTest] enter";

        // auto* cs = ctx.services.get<ComplexService>();
        //можно дописать
        //так как transient, то надо unique_ptr
        ComplexService* cs = static_cast<ComplexService*>(ctx.services.getRaw(typeid(ComplexService).hash_code()));

        qDebug() << "ComplexService sum =" << cs->sum();

        next(ctx);

        qDebug() << "[MiddlewareInjectTest] exit";
        delete cs;
    }
};


#endif // TEST3_H
