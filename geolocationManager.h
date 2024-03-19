#ifndef GEOLOCATIONMANAGER_H
#define GEOLOCATIONMANAGER_H

#include <QObject>
#include "ipstackManager.h"
#include "databaseManager.h"

class GeolocationManager : public QObject
{
    Q_OBJECT
public:
    explicit GeolocationManager( std::shared_ptr<IPStackManager> ipstackMan, std::shared_ptr<DatabaseManager> dataMan ,QObject *parent = nullptr);
    explicit GeolocationManager(QObject *parent = nullptr);
    Q_INVOKABLE void addEntry(const QString &ipOrUrl);
    Q_INVOKABLE void deleteEntry(const QString &ipOrUrl);
    Q_INVOKABLE void retrieveEntry(const QString &ipOrUrl);
    Q_INVOKABLE void getAllEntries();

signals:
    void dataReceived(const QString &data);
    void errorOccurred(const QString &error);

private slots:
    void handleApiResponse(const QString &data);
    void handleError(const QString &error);

private:
    std::shared_ptr<IPStackManager> m_ipStackManager;
    std::shared_ptr<DatabaseManager> m_databaseManager;

    QString m_currentQuery;
    QString convertVariantListToJsonString(const QVariantList &list);
};

#endif // GEOLOCATIONMANAGER_H
