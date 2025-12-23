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
    std::optional<double> rate = std::nullopt;
    QString rateStr = "";

    if (from == to) {
        rate = 1.0;
        rateStr = QString::number(1.0, 'f', 6);
        return ExchangeData{from, to, rate, rateStr, QDateTime::currentDateTime()};
    }

    QString fromCode = Enums::toStr(from);
    QString toCode = Enums::toStr(to);

    {
        QUrl url("https://api.exchangerate.host/convert");
        QUrlQuery q;
        q.addQueryItem("from", fromCode);
        q.addQueryItem("to", toCode);
        q.addQueryItem("amount", "1");
        url.setQuery(q);

        QNetworkAccessManager mgr;
        QNetworkReply *reply = mgr.get(QNetworkRequest(url));
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() == QNetworkReply::NoError) {
            QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();
            QJsonValue infoVal = obj.value("info");
            if (infoVal.isObject()) {
                QJsonValue rateVal = infoVal.toObject().value("rate");
                if (rateVal.isDouble()) {
                    rate = rateVal.toDouble();
                    rateStr = QString::number(*rate, 'f', 6);
                }
            }
        }
        reply->deleteLater();
    }

    if (!rate.has_value()) {
        QUrl url2(QString("https://open.er-api.com/v6/latest/%1").arg(fromCode));
        QNetworkAccessManager mgr2;
        QNetworkReply *reply2 = mgr2.get(QNetworkRequest(url2));
        QEventLoop loop2;
        QObject::connect(reply2, &QNetworkReply::finished, &loop2, &QEventLoop::quit);
        loop2.exec();

        if (reply2->error() == QNetworkReply::NoError) {
            QJsonObject obj2 = QJsonDocument::fromJson(reply2->readAll()).object();
            QJsonObject ratesObj = obj2.value("rates").toObject();
            QJsonValue rateVal2 = ratesObj.value(toCode);

            if (rateVal2.isDouble()) { // Убрали toDouble(1.0)
                rate = rateVal2.toDouble();
                rateStr = QString::number(*rate, 'f', 6);
            }
        }
        reply2->deleteLater();
    }

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
