#include "contextwrapper.h"

std::pair<QUuid, QByteArray> ContextWrapper::reduceContext(MessageContext &msgCtx)
{
    return std::make_pair(msgCtx.connection, QJsonDocument(msgCtx.jsonResponce).toJson(QJsonDocument::JsonFormat::Compact));
}

std::optional<MessageContext> ContextWrapper::createContext(QUuid connection, QByteArray rawData,
                                                            EndpointRegistry& endpoints,
                                                            ServiceScope services)
{
    QJsonDocument doc = QJsonDocument::fromJson(rawData);
    if (!doc.isNull())
    {
        if (doc.isObject())
        {
            QJsonObject obj = doc.object();
            MessageContext ctx(std::move(services), endpoints);
            ctx.jsonRequest = obj;
            ctx.requestId = QUuid::createUuid();
            ctx.connection = connection;
            return ctx;
        }
    }
    return std::nullopt;
}
