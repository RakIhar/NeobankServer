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
    
public:
    void invoke(MessageContext& ctx, const RequestDelegate& next) override {
        if (!ctx.isAborted)
        {
            try
            {
                qDebug() << "[Authorization] enter";
                using namespace Common;
                QString prType = ctx.jsonRequest[toStr(JsonField::Type)].toString();
                
                if (requiresAuthentication(prType)) {
                    if (!ctx.session.isAvailable)
                    {
                        qDebug() << "[Authorization] Authentication required but session not available";
                        ctx.jsonResponce[toStr(JsonField::Type)] = toStr(ProtocolType::Authorization);
                        ctx.jsonResponce[toStr(JsonField::Result)] = false;
                        ctx.jsonResponce[toStr(JsonField::Reason)] = "Authentication required";
                        ctx.jsonResponce[toStr(JsonField::ReasonCode)] = "UNAUTHORIZED";
                        ctx.abort();
                        return;
                    }

                    if (!ctx.user.id)
                    {
                        qDebug() << "[Authorization] User not found in context";
                        ctx.jsonResponce[toStr(JsonField::Type)] = toStr(ProtocolType::Authorization);
                        ctx.jsonResponce[toStr(JsonField::Result)] = false;
                        ctx.jsonResponce[toStr(JsonField::Reason)] = "User not found";
                        ctx.jsonResponce[toStr(JsonField::ReasonCode)] = "UNAUTHORIZED";
                        ctx.abort();
                        return;
                    }
                    
                    qDebug() << "[Authorization] User authenticated, user_id:" << ctx.items["user_id"].toLongLong();
                    
                    // Проверка ролей
                    // Role requiredRole = getRequiredRole(endpointName);
                    // if (!hasRole(ctx, requiredRole)) {
                    //     ctx.jsonResponce["success"] = false;
                    //     ctx.jsonResponce["error"] = "Insufficient permissions";
                    //     ctx.abort();
                    //     return;
                    // }
                }
                
                next(ctx);
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

}

#endif // AUTHORIZATION_H
