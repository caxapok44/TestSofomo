#ifndef IIPSTACKMANAGER_H
#define IIPSTACKMANAGER_H

#include <QObject>
#include <QStringList>

class IIPStackManager : public QObject {
    Q_OBJECT

public:
    explicit IIPStackManager(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~IIPStackManager() {}

    virtual void standardLookup(const QString &ip) = 0;
    virtual void bulkLookup(const QStringList &ips) = 0;
    virtual void requesterLookup() = 0;

signals:
    virtual void geolocationDataReceived(const QString &data) = 0;
    virtual void errorOccurred(const QString &error) = 0;
};

#endif // IIPSTACKMANAGER_H
