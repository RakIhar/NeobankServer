#ifndef IENDPOINT_H
#define IENDPOINT_H
#include "../context/messagecontext.h"
#include "../common/constants.h"

using namespace Common;
class IEndpoint
{
protected:
    virtual void privateInvoke(MessageContext& ctx) = 0;

    virtual void errorResponceTemplate(QJsonObject& responce, QString reason)
    {
        responce[toStr(JsonField::Type)]   = toStr(prType());
        responce[toStr(JsonField::Result)] = false;
        responce[toStr(JsonField::Error)] = reason;
    };

    virtual void successResponceTemplate(QJsonObject& responce)
    {
        responce[toStr(JsonField::Type)]   = toStr(prType());
        responce[toStr(JsonField::Result)] = true;
    };

    virtual bool init(MessageContext& ctx){ return true; };

public:
    virtual ~IEndpoint() = default;

    virtual QString name() const = 0;

    virtual Common::ProtocolType prType() const = 0;

    void invoke(MessageContext& ctx)
    {
        if (!ctx.isAborted)
        {
            try
            {
                qDebug() << QString("[%1 endpoint] enter").arg(name());
                if (init(ctx))
                    privateInvoke(ctx);
                else
                {
                    errorResponceTemplate(ctx.jsonResponce, "[%1 endpoint] init error");
                    qDebug() << QString("[%1 endpoint] init error").arg(name());
                }

                qDebug() << QString("[%1 endpoint] exit").arg(name());
            }
            catch (...)
            {
                qDebug() << QString("[%1 endpoint] abort").arg(name());
                ctx.abort();
            }
        }
    };
};

class EndpointRegistry
{
public:
    void registerEndpoint(const QString& name, std::unique_ptr<IEndpoint> ep)
    { endpoints[name] = std::move(ep); }

    IEndpoint* getEndpoint(const QString& name)
    {
        auto it = endpoints.find(name);
        if (it == endpoints.end())
            return nullptr;

        return it->second.get();
    }
private:
    std::unordered_map<QString, std::unique_ptr<IEndpoint>> endpoints;
};

#endif // IENDPOINT_H
