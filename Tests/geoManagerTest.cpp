#include "geoManagerTest.h"

void GeolocationManagerTest::initTestCase() {

    manager = std::make_shared<GeolocationManager>();
}

void GeolocationManagerTest::testAddEntry_NewEntry_Success(){}
void GeolocationManagerTest::testAddEntry_AlreadyExists_Error(){}
void GeolocationManagerTest::testDeleteEntry_Exists_Success(){}
void GeolocationManagerTest::testDeleteEntry_NotExists_Error(){}
void GeolocationManagerTest::testRetrieveEntry_Exists_Success(){}
void GeolocationManagerTest::testRetrieveEntry_NotExists_Error(){}
void GeolocationManagerTest::testGetAllEntries_EntriesExist_Success(){}
void GeolocationManagerTest::testGetAllEntries_NoEntries_Error(){}

void GeolocationManagerTest::cleanupTestCase() {

}

QTEST_MAIN(GeolocationManagerTest)

