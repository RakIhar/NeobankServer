#ifndef TRANSACTIONPROCESSOR_H
#define TRANSACTIONPROCESSOR_H

#include <QObject>

class TransactionProcessor : public QObject
{
    Q_OBJECT
public:
    explicit TransactionProcessor(QObject *parent = nullptr);

signals:
};

#endif // TRANSACTIONPROCESSOR_H
