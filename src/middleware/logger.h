#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include "../pipeline/middleware.h"

class Logger : public QObject, public IMiddleware
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = nullptr);

signals:
};

#endif // LOGGER_H
