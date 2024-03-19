#include <QObject>
#include <QtTest/QtTest>
#include "geolocationManager.h"

class GeolocationManagerTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testAddEntry_NewEntry_Success();
    void testAddEntry_AlreadyExists_Error();
    void testDeleteEntry_Exists_Success();
    void testDeleteEntry_NotExists_Error();
    void testRetrieveEntry_Exists_Success();
    void testRetrieveEntry_NotExists_Error();
    void testGetAllEntries_EntriesExist_Success();
    void testGetAllEntries_NoEntries_Error();
    void cleanupTestCase();

private:
    std::shared_ptr<GeolocationManager> manager;
};
