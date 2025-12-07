#include "authorization.h"

namespace Middlewares {

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

}
