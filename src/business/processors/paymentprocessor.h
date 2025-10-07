#ifndef PAYMENTPROCESSOR_H
#define PAYMENTPROCESSOR_H

#include <QObject>

class PaymentProcessor : public QObject
{
    Q_OBJECT
public:
    explicit PaymentProcessor(QObject *parent = nullptr);

signals:
};

#endif // PAYMENTPROCESSOR_H
