#ifndef MIDDLEWARE_AUTHENTIFICATION_H
#define MIDDLEWARE_AUTHENTIFICATION_H

#include "imiddleware.h"
#include "../service/authentification.h"

namespace Middlewares{

class Authentification : public IMiddleware
{
public:
    void invoke(MessageContext& ctx, const RequestDelegate& next) override {
        if (!ctx.isAborted)
        {
            try
            {
                qDebug() << "[Authentification] enter";
                //логика, вызов сервисов
                Services::Authentification* a = static_cast<Services::Authentification*>(ctx.services.getRaw(typeid(Services::Authentification).hash_code()));

                next(ctx);
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
//AuthorizeAttribute - для авторизации

/*
Клиент отправляет логин/пароль.
Сервер проверяет и создаёт серверную сессию (в БД, в памяти, Redis).
Сервер отвечает SetSessionId: sessionId=XYZ; HttpOnly; Secure. - sessionId можно отменить, это нормальный идентификатор
Клиент автоматически отправляет это cookie в каждом запросе.
Сервер берёт sessionId → находит запись в storage → узнаёт пользователя.
*/
