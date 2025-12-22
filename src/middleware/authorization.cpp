#include "authorization.h"
#include "../common/constants.h"
using namespace Common;
namespace Middlewares
{

const QStringList Authorization::publicEndpoints = {
    "login",
    "register",
    "health",
    "ping"
};

bool Authorization::isPublicEndpoint(const QString& protocolType) const
{
    return publicEndpoints.contains(protocolType, Qt::CaseInsensitive);
}

bool Authorization::requiresAuthentication(const QString& protocolType) const
{
    if (protocolType.isEmpty()) {
        return true;
    }
    
    return !isPublicEndpoint(protocolType);
}

bool Authorization::isAuthorized(MessageContext &ctx)
{
    using namespace Common;
    QString prType = ctx.jsonRequest[toStr(JsonField::Type)].toString();

    if (requiresAuthentication(prType)) {
        if (!ctx.session.isAvailable)
        {
            qDebug() << "[Authorization] Authentication required but session not available";
            ctx.jsonResponce[toStr(JsonField::Type)] = toStr(ProtocolType::Authorization);
            ctx.jsonResponce[toStr(JsonField::Result)] = false;
            ctx.jsonResponce[toStr(JsonField::Error)] = "Authentication required";
            ctx.jsonResponce[toStr(JsonField::ReasonCode)] = "UNAUTHORIZED";

            return false;
        }

        if (!ctx.user.user_id)
        {
            qDebug() << "[Authorization] User not found in context";
            ctx.jsonResponce[toStr(JsonField::Type)] = toStr(ProtocolType::Authorization);
            ctx.jsonResponce[toStr(JsonField::Result)] = false;
            ctx.jsonResponce[toStr(JsonField::Error)] = "User not found";
            ctx.jsonResponce[toStr(JsonField::ReasonCode)] = "UNAUTHORIZED";
            return false;
        }

        qDebug() << "[Authorization] User authenticated, user_id:" << ctx.user.user_id;

        /*
        Role requiredRole = getRequiredRole(endpointName);
        if (!hasRole(ctx, requiredRole)) {
            ctx.jsonResponce["success"] = false;
            ctx.jsonResponce["error"] = "Insufficient permissions";
            ctx.abort();
            return;
        }*/
    }
    return true;
}

}
