#include "currencyexchange.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
using namespace Services;
ExchangeData CurrencyExchange::get(Enums::Currency from, Enums::Currency to)
{
    // Try external free API: exchangerate.host convert endpoint.
    QString fromStr = Enums::toStr(from);
    QString toStr = Enums::toStr(to);
    QUrl url("https://api.exchangerate.host/convert");
    QUrlQuery q;
    q.addQueryItem("from", fromStr);
    q.addQueryItem("to", toStr);
    q.addQueryItem("amount", "1");
    url.setQuery(q);

    QNetworkAccessManager mgr;
    QNetworkRequest req(url);
    QNetworkReply *reply = mgr.get(req);
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    double rate = 1.0;
    QString rateStr = "1.000000";
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            if (obj.contains("info") && obj.value("info").isObject()) {
                QJsonObject info = obj.value("info").toObject();
                rate = info.value("rate").toDouble(1.0);
                rateStr = QString::number(rate, 'f', 6);
            }
        }
    }
    // if convert returned 1.0 for different currencies, try fallback provider
    bool needFallback = (rate == 1.0 && from != to);
    reply->deleteLater();

    if (needFallback) {
        // try open.er-api.com (public) to fetch rates for 'from'
        QString fromCode = Enums::toStr(from);
        QString toCode = Enums::toStr(to);
        QUrl url2(QString("https://open.er-api.com/v6/latest/%1").arg(fromCode));
        QNetworkAccessManager mgr2;
        QNetworkRequest req2(url2);
        QNetworkReply *reply2 = mgr2.get(req2);
        QEventLoop loop2;
        QObject::connect(reply2, &QNetworkReply::finished, &loop2, &QEventLoop::quit);
        loop2.exec();
        if (reply2->error() == QNetworkReply::NoError) {
            QByteArray data2 = reply2->readAll();
            QJsonDocument doc2 = QJsonDocument::fromJson(data2);
            if (doc2.isObject()) {
                QJsonObject obj2 = doc2.object();
                if (obj2.contains("rates") && obj2.value("rates").isObject()) {
                    QJsonObject ratesObj = obj2.value("rates").toObject();
                    if (ratesObj.contains(toCode)) {
                        rate = ratesObj.value(toCode).toDouble(1.0);
                        rateStr = QString::number(rate, 'f', 6);
                    }
                }
            }
        }
        reply2->deleteLater();
    }

    // Fallback: if network failed, return 1
    return ExchangeData{from, to, rate, rateStr, QDateTime::currentDateTime()};
}

std::vector<std::vector<ExchangeData> > CurrencyExchange::getAll()
{
    QMetaEnum metaEnum = QMetaEnum::fromType<Enums::Currency>();
    const int count = metaEnum.keyCount();
    std::vector<std::vector<ExchangeData>> result(count, std::vector<ExchangeData>(count));
    for (int i = 0; i < count; ++i) {
        Enums::Currency valueI = static_cast<Enums::Currency>(metaEnum.value(i));
        for (int j = 0; j < count; ++j) {
            Enums::Currency valueJ = static_cast<Enums::Currency>(metaEnum.value(j));
            result[i][j] = get(valueI, valueJ);
        }
    }
    return result;
}

ExchangeData CurrencyExchange::get(Enums::Currency from, Enums::Currency to, QDateTime at)
{
    return ExchangeData {from, to, 1, "1.00", at};
}

std::vector<std::vector<ExchangeData> > CurrencyExchange::getAll(QDateTime at)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<Enums::Currency>();
    const int count = metaEnum.keyCount();
    std::vector<std::vector<ExchangeData>> result(count, std::vector<ExchangeData>(count));
    for (int i = 0; i < count; ++i) {
        Enums::Currency valueI = static_cast<Enums::Currency>(metaEnum.value(i));
        for (int j = 0; j < count; ++j) {
            Enums::Currency valueJ = static_cast<Enums::Currency>(metaEnum.value(j));
            result[i][j] = get(valueI, valueJ, at);
        }
    }
    return result;
}
