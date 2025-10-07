#ifndef AUDITOR_H
#define AUDITOR_H

#include <QObject>
//бизнес-логика аудита, что логировать
class Auditor : public QObject
{
    Q_OBJECT
public:
    explicit Auditor(QObject *parent = nullptr);

signals:
};

#endif // AUDITOR_H
