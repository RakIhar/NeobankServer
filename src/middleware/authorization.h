#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include "imiddleware.h"
#include <QStringList>
#include "../common/constants.h"
namespace Middlewares{

class Authorization : public IMiddleware
{
private:
    static const QStringList publicEndpoints;
    
    bool isPublicEndpoint(const QString& endpointName) const;
    
    bool requiresAuthentication(const QString& endpointName) const;
    
    bool isAuthorized(MessageContext& ctx);

public:
    void invoke(MessageContext& ctx, const RequestDelegate& next) override {
        if (!ctx.isAborted)
        {
            try
            {
                qDebug() << "[Authorization] enter";

                if(isAuthorized(ctx))
                    next(ctx);
                else
                    ctx.abort();

                qDebug() << "[Authorization] exit";
            }
            catch (...)
            {
                qDebug() << "[Authorization] abort";
                ctx.abort();
            }
        }
    }
};
/*std::optional<Models::User> userOpt = authService->getUserBySession(authSession.id);
            if (userOpt.has_value()) {
                Models::User dbUser = userOpt.value();
                ctx.user.user_id = dbUser.id;
                ctx.user.username = dbUser.username;
                //CHECK: бесполезно, это же endpoint, надо почекать в middleware
                //CHECK: мейби добавить полей в User
}*/
}

#endif // AUTHORIZATION_H
