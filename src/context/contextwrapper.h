#ifndef CONTEXTWRAPPER_H
#define CONTEXTWRAPPER_H

#include "context.h"

class ContextWrapper
{
public:
    ContextWrapper() = delete;
    static std::pair<QUuid, QByteArray> reduceContext(MessageContext &msgCtx);
    static std::optional<MessageContext> createContext(QUuid connection, QByteArray rawData,
                                                       EndpointRegistry& registry,
                                                       ServiceScope services);
};

#endif // CONTEXTWRAPPER_H
