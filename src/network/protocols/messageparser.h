#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include <QObject>
//логика парсинга, валидация, преобразование данных
//без слотов и сигналов
class MessageParser : public QObject
{
    Q_OBJECT
public:
    explicit MessageParser(QObject *parent = nullptr);

signals:
};

#endif // MESSAGEPARSER_H
