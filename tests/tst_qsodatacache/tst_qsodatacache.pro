QT += testlib \
      sql \
      widgets \
      core

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    ../../src/callsign.h \
    ../../src/database/database.h \
    ../../src/database/db_adif_primary_subdvisions_data.h \
    ../../src/database/queryexecutor.h \
    ../../src/frequency.h \
    ../../src/locator.h \
    ../../src/utilities.h \
    ../../src/qsodatacache.h

SOURCES +=  tst_qsodatacache.cpp \
    ../../src/callsign.cpp \
    ../../src/database/database.cpp \
    ../../src/database/db_adif_primary_subdvisions_data.cpp \
    ../../src/database/queryexecutor.cpp \
    ../../src/frequency.cpp \
    ../../src/locator.cpp \
    ../../src/utilities.cpp \
    ../../src/qsodatacache.cpp 

RESOURCES += resources.qrc

DEFINES += TESTING

# Define database location for testing
DEFINES += TEST_DATABASE=\\\"':memory:'\\\"

