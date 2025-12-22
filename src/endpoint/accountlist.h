#ifndef ENDPOINT_ACCOUNTLIST_H
#define ENDPOINT_ACCOUNTLIST_H
#include "iendpoint.h"
#include <QJsonArray>
#include "../common/constants.h"
#include "../common/serializers.h"
#include "../database/repositories/accountrepository.h"

namespace Endpoints
{
class AccountList : public IEndpoint
{
    void successResponce(QJsonObject& responce, const QList<Models::Account> accounts,
                         const int limit, const int page, const int total)
    {
        successResponceTemplate(responce);
        QJsonArray arr;
        for (const auto &acc : accounts)
        {
            QJsonObject obj;
            serialize(obj, acc);
            arr.append(obj);
        }
        responce[toStr(JsonField::AccArr)] = arr;
        responce[toStr(JsonField::Limit)] = limit;
        responce[toStr(JsonField::Page)]  = page;
        responce[toStr(JsonField::Count)] = total;
    }

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
    ProtocolType prType() const override { return ProtocolType::AccList; };
    QString name() const override { return "AccountList"; }
    void privateInvoke(MessageContext& ctx) override;
};
}

#endif // ENDPOINT_ACCOUNTLIST_H

