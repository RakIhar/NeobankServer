#ifndef REGISTERMANAGER_H
#define REGISTERMANAGER_H

#include <QObject>

class RegisterManager : public QObject
{
    Q_OBJECT
public:
    explicit RegisterManager(QObject *parent = nullptr);

signals:
};

#endif // REGISTERMANAGER_H
