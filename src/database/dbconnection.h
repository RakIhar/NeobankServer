#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <QObject>

class DBConnection : public QObject
{
    Q_OBJECT
public:
    explicit DBConnection(QObject *parent = nullptr);

signals:
};

#endif // DBCONNECTION_H
