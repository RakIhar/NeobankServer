#ifndef DDOSMANAGER_H
#define DDOSMANAGER_H

#include <QObject>

class DdosManager : public QObject
{
    Q_OBJECT
public:
    explicit DdosManager(QObject *parent = nullptr);

signals:
};

#endif // DDOSMANAGER_H
