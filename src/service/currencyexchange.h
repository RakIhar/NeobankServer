#ifndef CURRENCYEXCHANGE_SERVICE_H
#define CURRENCYEXCHANGE_SERVICE_H

#include "iservice.h"
#include "../common/constants.h"

namespace Services {

class CurrencyExchange : public IService
{
public:
    explicit CurrencyExchange() {}
    ExchangeData get(Enums::Currency from, Enums::Currency to);;
    ExchangeData get(Enums::Currency from, Enums::Currency to, QDateTime at);;
    std::vector<std::vector<ExchangeData>> getAll();
    std::vector<std::vector<ExchangeData>> getAll(QDateTime at);
};

}
#endif // CURRENCYEXCHANGE_H
