#ifndef TOKENMANAGER_H
#define TOKENMANAGER_H

#include <QObject>

class TokenManager : public QObject
{
    Q_OBJECT
public:
    explicit TokenManager(QObject *parent = nullptr);

signals:
};

#endif // TOKENMANAGER_H
