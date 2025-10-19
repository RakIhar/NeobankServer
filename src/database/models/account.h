#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>

struct Account
{
    int id = 0;
    int userId = 0;
    double balance = 0.0;
    QString currency;
};

#endif // ACCOUNT_H
