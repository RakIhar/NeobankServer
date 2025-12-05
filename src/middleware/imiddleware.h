#ifndef IMIDDLEWARE_H
#define IMIDDLEWARE_H
#include <functional>
#include "../context/context.h"

using RequestDelegate = std::function<void(MessageContext&)>;

class IMiddleware {
public:
    virtual ~IMiddleware() = default;

    virtual void invoke(MessageContext& ctx, const RequestDelegate& next);
};

//using MiddlewareFactory = std::function<std::unique_ptr<IMiddleware>()>; //указатели на контруктор без параметров
//бесполезно, так как муторно и т.д., проще передавать уже готовые объекты
#endif // IMIDDLEWARE_H
