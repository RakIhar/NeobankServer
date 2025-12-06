#ifndef IMIDDLEWARE_H
#define IMIDDLEWARE_H
#include <functional>
#include "../context/context.h"

using RequestDelegate = std::function<void(MessageContext&)>;

class IMiddleware {
public:
    virtual ~IMiddleware() = default;

    virtual void invoke(MessageContext& ctx, const RequestDelegate& next){};
};

#endif // IMIDDLEWARE_H
