#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QObject>

class Transaction : public QObject
{
    Q_OBJECT
public:
    explicit Transaction(QObject *parent = nullptr);

signals:
};

#endif // TRANSACTION_H
