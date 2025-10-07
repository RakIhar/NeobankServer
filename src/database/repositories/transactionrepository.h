#ifndef TRANSACTIONREPOSITORY_H
#define TRANSACTIONREPOSITORY_H

#include <QObject>

class TransactionRepository : public QObject
{
    Q_OBJECT
public:
    explicit TransactionRepository(QObject *parent = nullptr);

signals:
};

#endif // TRANSACTIONREPOSITORY_H
