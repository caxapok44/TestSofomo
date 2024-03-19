#include "geolocationManager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
GeolocationManager::GeolocationManager(std::shared_ptr<IPStackManager> ipstackMan, std::shared_ptr<DatabaseManager> dataMan, QObject *parent)
    : QObject(parent),
      m_ipStackManager(ipstackMan),
      m_databaseManager(dataMan)
{
    connect(m_ipStackManager.get(), &IPStackManager::geolocationDataReceived,
            this, &GeolocationManager::handleApiResponse);
    connect(m_ipStackManager.get(), &IPStackManager::errorOccurred,
            this, &GeolocationManager::handleError);
    connect(m_databaseManager.get(), &DatabaseManager::errorOccurred,
            this, &GeolocationManager::handleError);
}

GeolocationManager::GeolocationManager(QObject *parent)
    : QObject(parent),
      m_ipStackManager(new IPStackManager()),
      m_databaseManager(new DatabaseManager(this))
{
    connect(m_ipStackManager.get(), &IPStackManager::geolocationDataReceived,
            this, &GeolocationManager::handleApiResponse);
    connect(m_ipStackManager.get(), &IPStackManager::errorOccurred,
            this, &GeolocationManager::handleError);
    connect(m_databaseManager.get(), &DatabaseManager::errorOccurred,
            this, &GeolocationManager::handleError);
}

void GeolocationManager::addEntry(const QString &ipOrUrl) {
    QString existingEntry = m_databaseManager->retrieveEntry(ipOrUrl);
    m_currentQuery = ipOrUrl;
    if (!existingEntry.isEmpty()) {
        emit errorOccurred("Entry already exists for " + ipOrUrl);
        return;
    }
    m_ipStackManager->standardLookup(ipOrUrl);
}


void GeolocationManager::handleApiResponse(const QString &data) {
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QString ip = jsonObject["ip"].toString();

    if(!m_databaseManager->addEntry(m_currentQuery, data)) {
        qWarning() << "Adding information to the database failed";
        emit errorOccurred("Adding information was failed");
    } else {
        QString dbData = m_databaseManager->retrieveEntry(ip);
        if (!dbData.isEmpty()) {
            emit dataReceived(dbData);
        } else {
            qWarning() << "Failed to retrieve information from the database for IP: " << ip;
            emit errorOccurred("Failed to retrieve information from the database for IP: " + ip);
        }
    }
}


void GeolocationManager::deleteEntry(const QString &ipOrUrl) {
    QString existingEntry = m_databaseManager->retrieveEntry(ipOrUrl);
    if (existingEntry.isEmpty()) {
        emit errorOccurred("Entry not exist " + ipOrUrl);
        return;
    }
    if(!m_databaseManager->deleteEntry(ipOrUrl))
    {
        qWarning() << "Delete information was failed";
        emit errorOccurred("Delete information was failed");
    }

    getAllEntries();
}

void GeolocationManager::retrieveEntry(const QString &ipOrUrl) {
    QString data = m_databaseManager->retrieveEntry(ipOrUrl);
    if (!data.isEmpty()) {
        emit dataReceived(data);
    }
    else{
        emit errorOccurred("No such entry");
    }
}

void GeolocationManager::getAllEntries() {
    QVariantList entries = m_databaseManager->getAllEntries();
    if (entries.isEmpty()) {
        emit dataReceived("");
        emit errorOccurred("No entries found in the database.");
        return;
    }

    QJsonArray jsonArray;
    for (const QVariant &entry : entries) {
        QJsonObject jsonObj = QJsonObject::fromVariantMap(entry.toMap());
        jsonArray.append(jsonObj);
    }

    QJsonDocument doc(jsonArray);
    QString jsonString = doc.toJson(QJsonDocument::Compact);

    emit dataReceived(jsonString);
}


void GeolocationManager::handleError(const QString &error) {
    emit errorOccurred(error);
}

QString GeolocationManager::convertVariantListToJsonString(const QVariantList &list) {
    QJsonArray jsonArray;

    for (const QVariant &item : list) {
        // Assuming each item in the list can be converted to a QJsonObject
        QJsonObject obj = item.toJsonObject();
        jsonArray.append(obj);
    }

    QJsonDocument doc(jsonArray);
    QString jsonString = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    return jsonString;
}
