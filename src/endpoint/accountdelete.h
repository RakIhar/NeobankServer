#ifndef ACCOUNTDELETE_H
#define ACCOUNTDELETE_H
#include "iendpoint.h"
#include "../database/repositories/accountrepository.h"

namespace Endpoints
{
class AccountDelete : public IEndpoint
{
    bool init(MessageContext& ctx) override
    {
        if (repo != nullptr)
            return true;
        repo = static_cast<Database::AccountRepository*>(
            ctx.services.getRaw(typeid(Database::AccountRepository).hash_code()));
        return repo != nullptr;
    };

    Database::AccountRepository* repo;
public:
    ProtocolType prType() const override { return ProtocolType::AccDelete; };
    QString name() const override { return "AccountDelete"; }
    void privateInvoke(MessageContext &ctx) override;
};
}

#endif // ACCOUNTDELETE_H
