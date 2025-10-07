#ifndef CERTIFICATEMANAGER_H
#define CERTIFICATEMANAGER_H

#include <QObject>
//работа с сертификатами - сертификаты X.509
class CertificateManager : public QObject
{
    Q_OBJECT
public:
    explicit CertificateManager(QObject *parent = nullptr);

signals:
};

#endif // CERTIFICATEMANAGER_H
