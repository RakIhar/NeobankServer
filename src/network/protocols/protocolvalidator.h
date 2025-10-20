#ifndef PROTOCOLVALIDATOR_H
#define PROTOCOLVALIDATOR_H
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class ProtocolValidator
{
public:
    ProtocolValidator() = delete;
    static bool validateMessage(const QByteArray &msg, QJsonDocument& doc);
private:
    static bool validateJsonObject(const QJsonObject& obj);
    static bool validateJsonArray(const QJsonArray& arr);
};

#endif // PROTOCOLVALIDATOR_H
