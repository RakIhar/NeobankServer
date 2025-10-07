#ifndef REPORTSERVICE_H
#define REPORTSERVICE_H

#include <QObject>

class ReportService : public QObject
{
    Q_OBJECT
public:
    explicit ReportService(QObject *parent = nullptr);

signals:
};

#endif // REPORTSERVICE_H
