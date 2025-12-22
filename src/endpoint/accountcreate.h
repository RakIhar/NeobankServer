#ifndef ACCOUNTCREATE_H
#define ACCOUNTCREATE_H
#include "iendpoint.h"
#include "../common/serializers.h"
#include "../database/repositories/accountrepository.h"

namespace Endpoints
{
class AccountCreate : public IEndpoint
{
    void successResponce(QJsonObject& responce, Models::Account acc)
    {
        successResponceTemplate(responce);
        QJsonObject accObj;
        serialize(accObj, acc);
        responce[toStr(JsonField::AccObj)]  = accObj;
    }

    bool init(MessageContext& ctx) override
    {
        if (repo != nullptr)
            return true;
        repo = static_cast<Database::AccountRepository*>(
            ctx.services.getRaw(typeid(Database::AccountRepository).hash_code()));
        return repo != nullptr;
    };

    Database::AccountRepository* repo = nullptr;
public:
    ProtocolType prType() const override { return ProtocolType::AccCreate; };
    QString name() const override { return "AccountCreate"; }
    void privateInvoke(MessageContext &ctx) override;
};
}

#endif // ACCOUNTCREATE_H
