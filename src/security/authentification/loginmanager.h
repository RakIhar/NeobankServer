#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include <QObject>

class LogInManager : public QObject
{
    Q_OBJECT
public:
    explicit LogInManager(QObject *parent = nullptr);

signals:
};

#endif // LOGINMANAGER_H
