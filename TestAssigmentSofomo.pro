QT += quick quickcontrols2 qml core gui sql network testlib

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        #Tests/geoManagerTest.cpp \
        Tests/ipstackManagerMock.cpp \
        databaseManager.cpp \
        geolocationManager.cpp \
        ipstackManager.cpp \
        main.cpp

RESOURCES += qml.qrc data/geolocation.db scripts/pre-install.sh

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    IIpstackManager.h \
    #Tests/geoManagerTest.h \
    Tests/ipstackManagerMock.h \
    databaseManager.h \
    geolocationManager.h \
    ipstackManager.h

# Define the path to the database file relative to the .pro file
DB_SOURCE_PATH = $$PWD/data/geolocation.db

# Define the target path in the build directory
DB_TARGET_PATH = $$OUT_PWD/geolocation.db

# Custom target to copy the database file
QMAKE_POST_LINK += $$quote(cp -f $$DB_SOURCE_PATH $$DB_TARGET_PATH)
