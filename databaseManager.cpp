#include "databaseManager.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QVariant>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlRecord>
#include <QCoreApplication>
#include <QDir>


DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{
    QString appDirPath = QCoreApplication::applicationDirPath();
    QString dbFileName = "geolocation.db";
    QString sourcePath = QDir(QCoreApplication::applicationDirPath()).filePath("data/" + dbFileName);
    QString targetPath = QDir(appDirPath).filePath(dbFileName);

    QFile dbFile(targetPath);
    qWarning() << dbFile.exists();
    qWarning() << sourcePath << " " << targetPath;
    if (!dbFile.exists()) {
        QFile::copy(sourcePath, targetPath);
    }
    if (!openDatabase(targetPath))
    {
        qWarning() << "Database could not be opened!";
    }
}

DatabaseManager::~DatabaseManager()
{
    closeDatabase();
}

bool DatabaseManager::openDatabase(const QString &path) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    if (!m_db.open()) {
        qWarning() << "Error: connection with database failed";
        return false;
    }

    if (!m_db.isValid()) {
        qWarning() << "Error: the database connection is not valid";
        return false;
    }

    if (!createTable()) {
        qWarning() << "Error: creation of table failed";
        return false;
    }

    return true;
}

bool DatabaseManager::createTable()
{
    QSqlQuery query;
    bool success = query.exec(
        "CREATE TABLE IF NOT EXISTS geolocation ("
        "ip TEXT PRIMARY KEY, "
        "url TEXT, "
        "type TEXT, "
        "continent_code TEXT, "
        "continent_name TEXT, "
        "country_code TEXT, "
        "country_name TEXT, "
        "region_code TEXT, "
        "region_name TEXT, "
        "city TEXT, "
        "zip TEXT, "
        "latitude REAL, "
        "longitude REAL, "
        "geoname_id INTEGER, "
        "capital TEXT, "
        "country_flag TEXT, "
        "calling_code TEXT, "
        "is_eu BOOLEAN)"
    );

    if (!success) {
        qWarning() << "Couldn't create the table 'geolocation':" << query.lastError().text();
    }
    return success;
}


bool DatabaseManager::addEntry(const QString &originalQuery, const QString &jsonData) {
    if (!m_db.isOpen() || !m_db.isValid()) {
            emit errorOccurred("Database connection is not open or valid.");
            return false;
        }

    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject obj = doc.object();

    // Check if the IP address is empty
    QString ip = obj["ip"].toString();
       // Check if the original query was an IP or URL and set appropriately
    QString url = (originalQuery != ip) ? originalQuery : "";
    if (ip.isEmpty()) {
        qWarning() << "IP address is empty, not adding entry";
        return false;  // Return false to indicate that the operation was not successful
    }

    QSqlQuery query;
    query.prepare(
        "INSERT INTO geolocation (ip, url, type, continent_code, continent_name, country_code, country_name, "
        "region_code, region_name, city, zip, latitude, longitude, geoname_id, capital, "
        "country_flag, calling_code, is_eu) "
        "VALUES (:ip, :url, :type, :continent_code, :continent_name, :country_code, :country_name, "
        ":region_code, :region_name, :city, :zip, :latitude, :longitude, :geoname_id, :capital, "
        ":country_flag, :calling_code, :is_eu)"
    );

    query.bindValue(":ip", ip);
    query.bindValue(":url", url);
    query.bindValue(":type", obj["type"].toString());
    query.bindValue(":continent_code", obj["continent_code"].toString());
    query.bindValue(":continent_name", obj["continent_name"].toString());
    query.bindValue(":country_code", obj["country_code"].toString());
    query.bindValue(":country_name", obj["country_name"].toString());
    query.bindValue(":region_code", obj["region_code"].toString());
    query.bindValue(":region_name", obj["region_name"].toString());
    query.bindValue(":city", obj["city"].toString());
    query.bindValue(":zip", obj["zip"].toString());
    query.bindValue(":latitude", obj["latitude"].toDouble());
    query.bindValue(":longitude", obj["longitude"].toDouble());
    QJsonObject location = obj["location"].toObject();
    query.bindValue(":geoname_id", location["geoname_id"].toInt());
    query.bindValue(":capital", location["capital"].toString());
    query.bindValue(":country_flag", location["country_flag"].toString());
    query.bindValue(":calling_code", location["calling_code"].toString());
    query.bindValue(":is_eu", location["is_eu"].toBool());

    if (!query.exec()) {
        qWarning() << "Add entry error:" << query.lastError().text();
        return false;
    }
    return true;
}



bool DatabaseManager::deleteEntry(const QString &ipOrUrl)
{
    if (!m_db.isOpen() || !m_db.isValid())
    {
        emit errorOccurred("Database connection is not open or valid.");
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM geolocation WHERE ip = :ipOrUrl or url = :ipOrUrl");
    query.bindValue(":ipOrUrl", ipOrUrl);
    if (!query.exec()) {
        qWarning() << "Delete entry error:" << query.lastError().text();
        emit errorOccurred("Delete entry error");
        return false;
    }
    return true;
}

QString DatabaseManager::retrieveEntry(const QString &ipOrUrl)
{
    if (!m_db.isOpen() || !m_db.isValid()) {
        emit errorOccurred("Database connection is not open or valid.");
        return QString();
    }


    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM geolocation WHERE ip = :ipOrUrl or url = :ipOrUrl");
    query.bindValue(":ipOrUrl", ipOrUrl);

    if (!query.exec()) {
        emit errorOccurred(query.lastError().text());
        return QString();
    }

    if (query.next()) {
        QJsonObject result;

        result["ip"] = query.value("ip").toString();
        result["url"] = query.value("url").toString();
        result["type"] = query.value("type").toString();
        result["continent_code"] = query.value("continent_code").toString();
        result["continent_name"] = query.value("continent_name").toString();
        result["country_code"] = query.value("country_code").toString();
        result["country_name"] = query.value("country_name").toString();
        result["region_code"] = query.value("region_code").toString();
        result["region_name"] = query.value("region_name").toString();
        result["city"] = query.value("city").toString();
        result["zip"] = query.value("zip").toString();
        result["latitude"] = query.value("latitude").toString();
        result["longitude"] = query.value("longitude").toString();
        result["geoname_id"] = query.value("geoname_id").toString();
        result["capital"] = query.value("capital").toString();
        result["country_flag"] = query.value("country_flag").toString();
        result["calling_code"] = query.value("calling_code").toString();
        result["is_eu"] = query.value("is_eu").toString();

        QJsonDocument doc(result);
        return QString(doc.toJson(QJsonDocument::Compact));
    }

    return QString();
}
QVariantList DatabaseManager::getAllEntries() {
    if (!m_db.isOpen() || !m_db.isValid()) {
            emit errorOccurred("Database connection is not open or valid.");
            return QVariantList();
    }
    QVariantList entriesList;

    QSqlQuery query("SELECT * FROM geolocation", m_db);
    while (query.next()) {
        QMap<QString, QVariant> map;
        for (int i = 0; i < query.record().count(); i++) {
            QString fieldName = query.record().fieldName(i);
            QVariant value = query.value(i);
            map[fieldName] = value;

            qDebug() << fieldName << ": " << value.toString();  // Debugging output
        }
        entriesList.append(map);
    }

    if (query.lastError().isValid()) {
        qWarning() << "Database error:" << query.lastError().text();
        return entriesList;
    }

    return entriesList;
}



void DatabaseManager::closeDatabase()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}
