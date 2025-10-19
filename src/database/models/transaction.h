#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QString>
#include <QDateTime>

struct Transaction
{
    int id = 0;
    int accountId = 0;
    double amount = 0.0;
    QString type;
    QDateTime timestamp;
};
#endif // TRANSACTION_H
