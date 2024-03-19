#include "ipstackManager.h"
#include <QUrl>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonDocument>

IPStackManager::IPStackManager()
    : m_networkAccessManager(new QNetworkAccessManager(this))
{
    m_apiKey = "855b24ff07cd3a8927b43047d69f19e1";
    connect(m_networkAccessManager, &QNetworkAccessManager::finished,
            this, &IPStackManager::onNetworkReply);
}

void IPStackManager::standardLookup(const QString &ip)
{
    QUrl url(QString("http://api.ipstack.com/%1").arg(ip));
    QUrlQuery query;
    query.addQueryItem("access_key", m_apiKey);
    url.setQuery(query);

    QNetworkRequest request(url);
    m_networkAccessManager->get(request);
}

void IPStackManager::bulkLookup(const QStringList &ips)
{
    QString ipList = ips.join(",");
    QUrl url("http://api.ipstack.com/" + ipList);
    QUrlQuery query;
    query.addQueryItem("access_key", m_apiKey);
    url.setQuery(query);

    QNetworkRequest request(url);
    m_networkAccessManager->get(request);
}

void IPStackManager::requesterLookup()
{
    QUrl url("http://api.ipstack.com/check");
    QUrlQuery query;
    query.addQueryItem("access_key", m_apiKey);
    url.setQuery(query);

    QNetworkRequest request(url);
    m_networkAccessManager->get(request);
}

void IPStackManager::onNetworkReply(QNetworkReply *reply) {
    if (!reply) {
        emit errorOccurred("Network reply is null");
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        QString response = reply->readAll();
        qWarning() << "IPStack API response:" << response;
        emit geolocationDataReceived(response);
    } else {
        QString errorStr = reply->errorString();
        qWarning() << "Network error occurred:" << errorStr;

        switch (reply->error()) {
            case QNetworkReply::ConnectionRefusedError:
            case QNetworkReply::RemoteHostClosedError:
            case QNetworkReply::HostNotFoundError:
            case QNetworkReply::TimeoutError:
            case QNetworkReply::NetworkSessionFailedError:
            case QNetworkReply::UnknownNetworkError:
                emit errorOccurred("Network error: " + errorStr);
                break;
            case QNetworkReply::OperationCanceledError:
                emit errorOccurred("Operation canceled: " + errorStr);
                break;
            default:
                emit errorOccurred("Error: " + errorStr);
                break;
        }
    }

    reply->deleteLater();
}

