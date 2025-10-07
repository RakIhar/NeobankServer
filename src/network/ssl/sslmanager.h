#ifndef SSLMANAGER_H
#define SSLMANAGER_H

#include <QObject>
//управление SSL/TLS соединениями
class SSLManager : public QObject
{
    Q_OBJECT
public:
    explicit SSLManager(QObject *parent = nullptr);

signals:
};

#endif // SSLMANAGER_H
