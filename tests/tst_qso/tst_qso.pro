QT += testlib \
      sql \
      widgets \
      core

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    ../../src/utilities.h \
    ../../src/callsign.h \
    ../../src/global.h \
    ../../src/klogdefinitions.h \
    ../../src/locator.h \
    ../../src/qso.h \    
    ../../src/adif.h

SOURCES +=  tst_qso.cpp \
    ../../src/utilities.cpp \
    ../../src/callsign.cpp \
    ../../src/locator.cpp \
    ../../src/qso.cpp \
    ../../src/adif.cpp


DEFINES += TESTING

# Define database location for testing
DEFINES += TEST_DATABASE=\\\"':memory:'\\\"

