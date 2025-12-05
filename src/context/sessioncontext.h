#ifndef SESSIONCONTEXT_H
#define SESSIONCONTEXT_H

// ISession (Id, IsAvailable, Data) - хранение данных между запросами, потому что MessageContext лишь на один запрос
class SessionContext
{
public:
    SessionContext();
};

#endif // SESSIONCONTEXT_H
