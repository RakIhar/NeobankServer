#include "contextwrapper.h"

ContextWrapper::ContextWrapper(QObject *parent)
    : QObject{parent}
{}

void ContextWrapper::onCreateContext(const QUuid &connection, const QByteArray &rawData)
{
    QJsonDocument doc = QJsonDocument::fromJson(rawData);
    if (!doc.isNull())
    {
        if (doc.isObject())
        {
            QJsonObject obj = doc.object();
            MessageContext* ctx = new MessageContext();
            ctx->jsonRequest = obj;
            ctx->requestId = QUuid::createUuid();
            ctx->connection = connection;
            emit contextCreated(ctx);
        }
    }

}

void ContextWrapper::onReduceContext(MessageContext *msgCtx)
{
    emit contextReduced(msgCtx->connection, QJsonDocument(msgCtx->jsonRequest).toJson());
}
