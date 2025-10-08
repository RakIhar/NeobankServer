#ifndef CLIENTSESSION_H
#define CLIENTSESSION_H

#include <QObject>
//slots:
//processData
//onSocket
class ClientSession : public QObject
{
    Q_OBJECT
public:
    explicit ClientSession(QObject *parent = nullptr);

signals:
};

#endif // CLIENTSESSION_H
