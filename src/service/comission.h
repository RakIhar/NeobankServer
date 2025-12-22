#ifndef COMISSIONSERVICE_H
#define COMISSIONSERVICE_H

#include "iservice.h"

namespace Services {

class Comission : public IService
{
public:
    explicit Comission() {}
    qint64 computeCommissionCents(qint64 amountCents) const;
    QString commissionPercentString() const;
private:
    const qint64 m_flatCents = 50;
    const double m_percent = 0.01;
};

}

#endif // COMISSION_H
