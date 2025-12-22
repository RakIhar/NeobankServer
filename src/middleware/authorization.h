#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H
#include "imiddleware.h"
#include <QStringList>

namespace Middlewares
{
class Authorization : public IMiddleware
{
private:
    static const QStringList publicEndpoints;
    
    bool isPublicEndpoint(const QString& endpointName) const;
    
    bool requiresAuthentication(const QString& endpointName) const;
    
    bool isAuthorized(MessageContext& ctx);

public:
    QString name() const override { return "Authorization"; }

    void privateInvoke(MessageContext& ctx, const RequestDelegate& next) override
    {
        if(isAuthorized(ctx))
            next(ctx);
        else
            ctx.abort();
    }
};
}

#endif // AUTHORIZATION_H
