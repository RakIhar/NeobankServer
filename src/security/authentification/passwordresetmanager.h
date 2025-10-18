#ifndef PASSWORDRESETMANAGER_H
#define PASSWORDRESETMANAGER_H

#include <QObject>

class PasswordResetManager : public QObject
{
    Q_OBJECT
public:
    explicit PasswordResetManager(QObject *parent = nullptr);

signals:
};

#endif // PASSWORDRESETMANAGER_H
