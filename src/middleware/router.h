#ifndef ROUTER_H
#define ROUTER_H

#include <QObject>
#include "imiddleware.h"
/*
class EndpointCollection {
public:
    void add(const QString& path, EndpointDelegate handler) {
        m_routes[path] = handler;
    }

    const EndpointDelegate* match(const QString& path) const {
        return m_routes.contains(path) ? &m_routes[path] : nullptr;
    }

private:
    QHash<QString, EndpointDelegate> m_routes; // живёт ВСЁ ВРЕМЯ
};


middleware -> ctx.RequestServices (scoped)
                          |
                          | resolve()
                          ↓
                   ScopedContainer
              (ищет в своём кэше scoped)
                          |
                          | не найден
                          ↓
                   RootContainer
              (возвращает singleton instance)
*/
class Router : public QObject, public IMiddleware
{
    Q_OBJECT
public:
    explicit Router(QObject *parent = nullptr);

signals:
};

#endif // ROUTER_H
