#ifndef USER_H
#define USER_H

#include <QString>

struct User
{
    int id = 0;
    QString username;
    QString passwordHash;
    QString email;
};

#endif // USER_H
