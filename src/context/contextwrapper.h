#ifndef CONTEXTWRAPPER_H
#define CONTEXTWRAPPER_H

#include <QObject>
#include "messagecontext.h"

class ContextWrapper : public QObject
{
    Q_OBJECT
public:
    explicit ContextWrapper(QObject *parent = nullptr);

signals:
    void contextCreated(MessageContext* msgCtx);
    void contextReduced(const QUuid &connection, const QByteArray &rawData);
public slots:
    void onCreateContext(const QUuid &connection, const QByteArray &rawData);
    void onReduceContext(MessageContext* msgCtx);
};

#endif // CONTEXTWRAPPER_H
