#ifndef LOGGERSERVICE_H
#define LOGGERSERVICE_H

#include "iservice.h"

enum class LogType : uint32_t {
    None        = 0,
    InfoAll     = 1 << 0,
    InfoPrCompr = 1 << 1,
    InfoPrFull  = 1 << 2,
    InfoDb      = 1 << 3,
    InfoOther   = 1 << 4,
    Debug       = 1 << 5,
    Warning     = 1 << 6,
    Error       = 1 << 7,
    Critical    = 1 << 8
};
/*
Опции:
все info уже включают в себя InfoAll
все logType уже включают в себя общий логовый файл
qDebug()
qCritical()
qWarning()
qInfo()
qFatal() вроде их можно интегрировать
*/
class LoggerService : public IService
{
public:
    LoggerService();
    // void log(const std::string& message, LogLevel level = LogLevel::Info);
    // void debug(const std::string& msg) {
        // qDebug();
        // qWarning();
        // qCritical();
        // log(msg, LogLevel::Debug); }
    // void error(const std::string& msg) { log(msg, LogLevel::Error); }
};

#endif // LOGGERSERVICE_H
