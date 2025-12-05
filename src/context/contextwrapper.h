#ifndef CONTEXTWRAPPER_H
#define CONTEXTWRAPPER_H

#include <QObject>
#include "context.h"

class ContextWrapper
{
public:
    ContextWrapper() = delete;
    static std::pair<QUuid, QByteArray> reduceContext(MessageContext &msgCtx);
    static std::optional<MessageContext> createContext(QUuid &connection, QByteArray &rawData,
                                                       EndpointRegistry& registry,
                                                       ServiceScope& services);

signals:
    void contextCreated(MessageContext &msgCtx);
    void contextReduced(const QUuid &connection, const QByteArray &rawData);
public slots:
    void onCreateContext(const QUuid &connection, const QByteArray &rawData);
    void onReduceContext(MessageContext &msgCtx);
};

#endif // CONTEXTWRAPPER_H
