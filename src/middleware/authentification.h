#ifndef MIDDLEWARE_AUTHENTIFICATION_H
#define MIDDLEWARE_AUTHENTIFICATION_H

#include "imiddleware.h"
#include "../service/authentification.h"
#include "../service/session.h"
#include "../common/constants.h"

#include <QJsonObject>
#include <QUuid>

namespace Middlewares{

class Authentification : public IMiddleware
{


    void authenticate(MessageContext& ctx, Services::Authentification* authService, QJsonObject& request);
public:

    void invoke(MessageContext& ctx, const RequestDelegate& next) override {
        if (!ctx.isAborted)
        {
            try
            {
                qDebug() << "[Authentification] enter";
                
                Services::Authentification* authService = static_cast<Services::Authentification*>(
                    ctx.services.getRaw(typeid(Services::Authentification).hash_code()));
                Services::Session* sessionService = static_cast<Services::Session*>(
                    ctx.services.getRaw(typeid(Services::Session).hash_code()));
                
                if (!authService || !sessionService) {
                    qDebug() << "[Authentification] Services not available";
                    ctx.abort();
                    return;
                }
                QJsonObject& request = ctx.jsonRequest;
                // if (isLoginRequest(request)) //вынести в endpoint
                // {
                //     handleLoginRequest(ctx, authService, request);
                // }
                // else
                // {
                authenticate(ctx, authService, request);
                // }
                
                next(ctx); //Нельзя прерывать, не все mdlware/endpts требуют аутентификации
                qDebug() << "[Authentification] exit";
            }
            catch (...)
            {
                qDebug() << "[Authentification] abort";
                ctx.abort();
            }
        }
    }
};

}

#endif // AUTHENTIFICATION_H

