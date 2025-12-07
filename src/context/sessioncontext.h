#ifndef SESSIONCONTEXT_H
#define SESSIONCONTEXT_H

// ISession (Id, IsAvailable, Data) - хранение данных между запросами
//должен быть сервис, этими сессиями управляющий(unique_ptr) - контекст будет хранить QPointer либо *
class SessionContext
{
public:
    SessionContext();
};

#endif // SESSIONCONTEXT_H
