#ifndef AESENCRYPTOR_H
#define AESENCRYPTOR_H

#include <QObject>

class AESEncryptor : public QObject
{
    Q_OBJECT
public:
    explicit AESEncryptor(QObject *parent = nullptr);

signals:
};

#endif // AESENCRYPTOR_H
