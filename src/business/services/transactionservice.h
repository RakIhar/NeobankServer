#ifndef TRANSACTIONSERVICE_H
#define TRANSACTIONSERVICE_H

#include <QObject>

class TransactionService : public QObject
{
    Q_OBJECT
public:
    explicit TransactionService(QObject *parent = nullptr);

signals:
};

#endif // TRANSACTIONSERVICE_H
