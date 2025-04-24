QT += testlib \
      sql \
      widgets \
      core

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    ../../src/database.h \
    ../../src/database/db_adif_primary_subdvisions_data.h \
    ../../src/database/queryexecutor.h \
    ../../src/utilities.h \
    ../../src/callsign.h \
    ../../src/frequency.h \
    ../../src/global.h \
    ../../src/klogdefinitions.h \
    ../../src/locator.h \
    ../../src/qso.h \    
    ../../src/adif.h

SOURCES +=  tst_qso.cpp \
    ../../src/database.cpp \
    ../../src/database/db_adif_primary_subdvisions_data.cpp \
    ../../src/database/queryexecutor.cpp \
    ../../src/utilities.cpp \
    ../../src/callsign.cpp \
    ../../src/frequency.cpp \
    ../../src/locator.cpp \
    ../../src/qso.cpp \
    ../../src/adif.cpp


DEFINES += TESTING

# Define database location for testing
DEFINES += TEST_DATABASE=\\\"':memory:'\\\"

