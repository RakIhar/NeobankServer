#ifndef SERVICE_SESSION_H
#define SERVICE_SESSION_H
#include "iservice.h"
#include "../database/repositories/authsessionrepository.h"
#include "../context/sessioncontext.h"

namespace Services {

class Session : public IService
{
    Database::AuthSessionRepository* m_repAuthSession;
    // QHash<QUuid, std::unique_ptr<SessionContext>> sessions;
    std::unordered_map<QString, std::unique_ptr<Context::Session>> sessions;
public:
    explicit Session(Database::AuthSessionRepository* repAuthSession)
        : m_repAuthSession(repAuthSession){};

    Context::Session* createSession()
    {
        std::unique_ptr<Context::Session> ctx = std::make_unique<Context::Session>();
        ctx->sessionId = QUuid::createUuid();
        ctx->isAvailable = true;
        Context::Session* result = ctx.get();
        sessions[ctx->sessionId.toString()] = std::move(ctx);
        return result;
    }

    Context::Session* getSession(QUuid sessionId)
    {
        auto it = sessions.find(sessionId.toString());
        if (it == sessions.end())
            return nullptr;
        return sessions[sessionId.toString()].get();
    }

    void removeSession(QUuid sessionId) //NEW
    {
        sessions.erase(sessionId.toString());
    }

    bool hasSession(QUuid sessionId) const //NEW
    {
        auto it = sessions.find(sessionId.toString());
        return it != sessions.end();
    }

};

}

#endif // SESSION_H
