#ifndef CONTEXT_USER_H
#define CONTEXT_USER_H
#include "QString"

//отдельная структура: identities+все связанное, IsInRole(?)
//identity: AuthenticationType, IsAuthenticated, Name, Roles, Claims(:type, value, valuetype, issuer, originalissuer)

namespace Context {

class User
{
public:
    User(){};
    qint64 id = 0;
    QString username;
};

}

#endif // USER_H
