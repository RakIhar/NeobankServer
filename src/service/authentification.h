#ifndef SERVICE_AUTHENTIFICATION_H
#define SERVICE_AUTHENTIFICATION_H
#include "iservice.h"
#include "../database/repositories/authsessionrepository.h"
#include "../database/repositories/userrepository.h"
#include "../database/models/authsession.h"
#include "../database/models/user.h"
#include <optional>
#include <QUuid>
#include <QString>
#include <QDateTime>

namespace Services {

class Authentification : public IService
{
    Database::AuthSessionRepository* m_authSessionRepo;
    Database::UserRepository* m_userRepo;
    
    static QString hashPassword(const QString& password);
    static QString generateToken();
    static bool verifyPassword(const QString& password, const QString& passwordHash);
    
public:
    explicit Authentification(Database::AuthSessionRepository* authSessionRepo, 
                              Database::UserRepository* userRepo) 
        : m_authSessionRepo(authSessionRepo), m_userRepo(userRepo) {};
    
    std::optional<Models::AuthSession> authenticate(const QString& username, const QString& password,
                                             const QString& ipAddress = "", 
                                             const QString& userAgent = "");
    
    std::optional<Models::AuthSession> validateSession(const QUuid& sessionId);
    
    std::optional<Models::AuthSession> validateSessionByToken(const QString& token);
    
    bool logout(const QUuid& sessionId);
    
    std::optional<Models::User> getUserBySession(const QUuid& sessionId);
};

}

#endif // AUTHENTIFICATION_H
