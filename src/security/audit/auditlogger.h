#ifndef AUDITLOGGER_H
#define AUDITLOGGER_H

#include <QObject>
//как логировать
class AuditLogger : public QObject
{
    Q_OBJECT
public:
    explicit AuditLogger(QObject *parent = nullptr);

signals:
};

#endif // AUDITLOGGER_H
