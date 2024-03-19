#include "ipstackManagerMock.h"
#include <QUrl>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonDocument>

IPStackManagerMock::IPStackManagerMock()
    :  m_networkAccessManager(new QNetworkAccessManager(this))
{
    connect(this, &IPStackManagerMock::mockFinished,
            this, &IPStackManagerMock::onNetworkReply);
}

void IPStackManagerMock::standardLookup(const QString &ip)
{
    emit mockFinished(nullptr);
}

void IPStackManagerMock::bulkLookup(const QStringList &ips)
{
     emit mockFinished(nullptr);
}

void IPStackManagerMock::requesterLookup()
{
     emit mockFinished(nullptr);
}

void IPStackManagerMock::onNetworkReply(QNetworkReply *reply)
{

    QString response = "{\"ip\": \"142.251.167.100\", \"type\": \"ipv4\", \"continent_code\": \"NA\", \"continent_name\": \"North America\", \"country_code\": \"US\", \"country_name\": \"United States\", \"region_code\": \"CA\", \"region_name\": \"California\", \"city\": \"Mountain View\", \"zip\": \"94043\", \"latitude\": 37.419158935546875, \"longitude\": -122.07540893554688, \"location\": {\"geoname_id\": 5375480, \"capital\": \"Washington D.C.\", \"languages\": [{\"code\": \"en\", \"name\": \"English\", \"native\": \"English\"}], \"country_flag\": \"https://assets.ipstack.com/flags/us.svg\", \"country_flag_emoji\": \"\\ud83c\\uddfa\\ud83c\\uddf8\", \"country_flag_emoji_unicode\": \"U+1F1FA U+1F1F8\", \"calling_code\": \"1\", \"is_eu\": false}}";
    emit geolocationDataReceived(response);

}
