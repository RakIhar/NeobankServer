#ifndef CONVERTERS_H
#define CONVERTERS_H

#include <QString>
#include <QStringList>
#include <QRegularExpression>
class Money {
public:
    static qint64 toCents(const QString &amountStr) {
        if (amountStr.isEmpty())
            return 0;

        static const QRegularExpression re("[.,]");
        QStringList parts = amountStr.split(re);

        qint64 major = parts[0].toLongLong();
        qint64 minor = 0;

        if (parts.size() > 1) {
            QString minorStr = parts[1].left(2).leftJustified(2, '0');
            minor = minorStr.toLongLong();
        }

        return (major * 100) + (major >= 0 ? minor : -minor);
    }
    static QString fromCents(qint64 cents) {
        return QString("%1.%2")
        .arg(cents / 100)
            .arg(qAbs(cents % 100), 2, 10, QChar('0'));
    }
};

#endif // CONVERTERS_H
