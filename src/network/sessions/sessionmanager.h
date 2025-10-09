#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QObject>
#include "clientsession.h"
#include <QPointer>
#include <QSet>

//хранит все сессии
//создает и удаляет сессии
//управляет временем их жизни
class SessionManager : public QObject
{
    Q_OBJECT
public:
    explicit SessionManager(QObject *parent = nullptr);
private:
    QSet<QPointer<ClientSession>> m_sessions;
signals:
};

#endif // SESSIONMANAGER_H
