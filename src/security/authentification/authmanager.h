#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QObject>

class AuthManager : public QObject
{
    Q_OBJECT
public:
    explicit AuthManager(QObject *parent = nullptr);
    bool authenticate(const QString& username, const QString& password);
    //гораздо сложнее - выдавать токены, работать с бд, работать с сертификатами
signals:
};

#endif // AUTHMANAGER_H
