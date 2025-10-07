#ifndef APPLICATIONSERVER_H
#define APPLICATIONSERVER_H

#include <QObject>
//initialize
//start
//stop
//reloadconfig

//dependency injection ?? - указатели на менеджеров

//onHealthCheck - проверка здоровья системы
//onStatisticsUpdate - сбор статистики
class ApplicationServer : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationServer(QObject *parent = nullptr);

signals:
};

#endif // APPLICATIONSERVER_H
