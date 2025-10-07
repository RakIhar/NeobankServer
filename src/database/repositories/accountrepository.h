#ifndef ACCOUNTREPOSITORY_H
#define ACCOUNTREPOSITORY_H

#include <QObject>

class AccountRepository : public QObject
{
    Q_OBJECT
public:
    explicit AccountRepository(QObject *parent = nullptr);

signals:
};

#endif // ACCOUNTREPOSITORY_H
