#ifndef ACCOUNTSERVICE_H
#define ACCOUNTSERVICE_H

#include <QObject>

class AccountService : public QObject
{
    Q_OBJECT
public:
    explicit AccountService(QObject *parent = nullptr);

signals:
};

#endif // ACCOUNTSERVICE_H
