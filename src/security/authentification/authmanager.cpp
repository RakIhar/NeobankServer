#include "authmanager.h"

AuthManager::AuthManager(QObject *parent) : QObject{parent} {}

AuthManager *AuthManager::instance()
{
    static AuthManager inst;
    return &inst;
}

AuthStatus AuthManager::processStep(AuthContext &ctx, const QByteArray &message)
{
    if (ctx.state == AuthState::None)
    {
        QByteArray login; //из message
        QByteArray passwordHash; //из message
        ctx.login = login;
        ctx.passwordHash = passwordHash;
        if (verifyCredentials(ctx.login, ctx.passwordHash))
        {
            if (isOtpRequired(ctx.login))
            {
                ctx.state = AuthState::InProgress;
                return AuthStatus::Pending;
            }
            ctx.state = AuthState::Completed;
            return AuthStatus::Success;
        }
        ctx.state = AuthState::Failed;
        return AuthStatus::Fail;
    }
    if (ctx.state == AuthState::InProgress)
    {
        QByteArray otp;
        ctx.otp = otp;
        if (verifyOtp(ctx.login, ctx.otp))
        {
            ctx.state = AuthState::Completed;
            return AuthStatus::Success;
        }
        ctx.state = AuthState::Failed;
        return AuthStatus::Fail;
    }

    return AuthStatus::Fail;
}

bool AuthManager::verifyCredentials(const QByteArray &login, const QByteArray &password)
{
    return true;
}

bool AuthManager::isOtpRequired(const QByteArray &login)
{
    return true;
}

bool AuthManager::verifyOtp(const QByteArray &login, const QByteArray &otp)
{
    return true;
}
