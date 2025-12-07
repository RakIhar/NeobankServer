#ifndef SERVICE_AUTHENTIFICATION_H
#define SERVICE_AUTHENTIFICATION_H
#include "iservice.h"
#include "../database/repositories/authsessionrepository.h"

namespace Services {

class Authentification : public IService
{
    Database::AuthSessionRepository* m_db;
public:
    explicit Authentification(Database::AuthSessionRepository* db) : m_db(db) {};

};

}


#endif // AUTHENTIFICATION_H
