#ifndef SESSION_H
#define SESSION_H

// ISession (Id, IsAvailable, Data) - хранение данных между запросами, потому что MessageContext лишь на один запрос
class Session
{
public:
    Session();
};

#endif // SESSION_H
