#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include "imiddleware.h"

class Session : public QObject, public IMiddleware
{
    Q_OBJECT
public:
    explicit Session(QObject *parent = nullptr);

signals:
};

#endif // SESSION_H
