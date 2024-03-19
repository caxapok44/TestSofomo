#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    bool openDatabase(const QString &path);
    bool addEntry(const QString &originalQuery, const QString &jsonData);
    bool deleteEntry(const QString &ip);
    QString retrieveEntry(const QString &ip);
    QVariantList getAllEntries();
    void closeDatabase();

signals:
    void errorOccurred(const QString &error);

private:
    QSqlDatabase m_db;
    bool createTable();
};

#endif // DATABASEMANAGER_H
