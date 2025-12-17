#ifndef SESSION_H
#define SESSION_H

#include "imiddleware.h"

#include "../service/session.h"

namespace Middlewares{

class Session : public IMiddleware
{
public:
    void invoke(MessageContext& ctx, const RequestDelegate& next) override {
        if (!ctx.isAborted)
        {
            try
            {
                qDebug() << "[Session] enter";
                //TODO
                //есть проблемы с разграничением auth_session и logic_session - в коде оба перепутаны, поэтому пофиг
                next(ctx);
                qDebug() << "[Session] exit";
            }
            catch (...)
            {
                qDebug() << "[Session] abort";
                ctx.abort();
            }
        }
    }
};

}

#endif // SESSION_H
