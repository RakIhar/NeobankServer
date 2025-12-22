#ifndef CONVERTERS_H
#define CONVERTERS_H

#include <QString>
#include <QStringList>
#include <QRegularExpression>
class Money {
public:
    static inline bool check(const QString &amountStr, const bool& isSignAllowed)
    {
        static const QRegularExpression re(R"(^[0-9]{0,12}(\.[0-9]{0,2})?$)");
        static const QRegularExpression reSign(R"(^-?[0-9]{0,12}(\.[0-9]{0,2})?$)");
        bool isCorrect;
        if (isSignAllowed)
            isCorrect = reSign.match(amountStr).hasMatch();
        else
            isCorrect = re.match(amountStr).hasMatch();
        if (!isCorrect)
            qDebug() << "Money::check incorrect, sign: " << isSignAllowed << ", " << amountStr;
        return isCorrect;
    }

    static inline std::pair<qint64, bool> toCents(const QString &amountStr, const bool& isSignAllowed = true)
    {
        if (!check(amountStr, isSignAllowed))
            return std::make_pair(0, false);

        static const QRegularExpression re("[.,]");
        QStringList parts = amountStr.split(re);

        qint64 major = parts[0].toLongLong();
        qint64 minor = 0;

        if (parts.size() > 1) {
            QString minorStr = parts[1].left(2).leftJustified(2, '0');//первые два левых символа, заполняется '0', если не хватает
            minor = minorStr.toLongLong();
        }
        qint64 res = (major * 100) + (major >= 0 ? minor : -minor);
        return std::make_pair(res, true);
    }
    static inline QString fromCents(qint64 cents)
    {
        //минимум 2 символа, 10-чная СС, заполняется '0', если не хватает
        return QString("%1.%2").arg(cents / 100).arg(qAbs(cents % 100), 2, 10, QChar('0'));
    }
};

#endif // CONVERTERS_H
