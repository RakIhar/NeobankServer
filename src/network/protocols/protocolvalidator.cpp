#include "protocolvalidator.h"

bool ProtocolValidator::validateMessage(const QByteArray &msg, QJsonDocument& doc)
{
    doc = QJsonDocument::fromJson(msg);
    if (doc.isNull()) {
        return false;
    } else if (doc.isObject()) {
        QJsonObject obj = doc.object();
        return validateJsonObject(obj);
    } else if (doc.isArray()) {
        QJsonArray arr = doc.array();
        return validateJsonArray(arr);
    }
    return false;
}

bool ProtocolValidator::validateJsonObject(const QJsonObject &obj)
{
    return true;
}

bool ProtocolValidator::validateJsonArray(const QJsonArray &arr)
{
    return false;
}


