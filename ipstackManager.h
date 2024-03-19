#ifndef IPSTACKMANAGER_H
#define IPSTACKMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "IIpstackManager.h"

class IPStackManager : public IIPStackManager
{
    Q_OBJECT
public:
    explicit IPStackManager();
    void standardLookup(const QString &ip) override;
    void bulkLookup(const QStringList &ips) override;
    void requesterLookup() override;

signals:
    void geolocationDataReceived(const QString &data) override;
    void errorOccurred(const QString &error) override;

private slots:
    void onNetworkReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_networkAccessManager;
    QString m_apiKey;
};

#endif // IPSTACKMANAGER_H
