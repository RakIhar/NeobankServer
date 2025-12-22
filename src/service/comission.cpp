#include "comission.h"
#include <cmath>

namespace Services {

qint64 Comission::computeCommissionCents(qint64 amountCents) const
{
    qint64 pct = static_cast<qint64>(std::llround(amountCents * m_percent));
    return m_flatCents + pct;
}

QString Comission::commissionPercentString() const
{
    double pct = m_percent * 100.0;
    QString pctStr = QString::number(pct, 'f', (qFuzzyCompare(pct, std::floor(pct)) ? 0 : 2));
    return QStringLiteral("комиссия %1%").arg(pctStr);
}

}


