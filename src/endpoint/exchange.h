#ifndef EXCHANGE_H
#define EXCHANGE_H
#include "iendpoint.h"
#include "../service/currencyexchange.h"

namespace Endpoints {

class ExchangeRates : public IEndpoint {
    bool init(MessageContext &ctx) override
    {
        if (exchange != nullptr) return true;
        exchange = static_cast<Services::CurrencyExchange*>(
            ctx.services.getRaw(typeid(Services::CurrencyExchange).hash_code()));
        return exchange != nullptr;
    }

    void privateInvoke(MessageContext &ctx) override;
    Services::CurrencyExchange *exchange = nullptr;
public:
    QString name() const override { return "exchange.rates"; }
    Common::ProtocolType prType() const override { return Common::ProtocolType::Undefined; }
};

}

#endif // EXCHANGE_H
