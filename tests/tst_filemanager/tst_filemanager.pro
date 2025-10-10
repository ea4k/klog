QT += testlib \
    gui \
    sql \
    widgets

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    ../../src/adif.h \
    ../../src/database/db_adif_primary_subdvisions_data.h \
    ../../src/awards.h \
    ../../src/awarddxmarathon.h \
    ../../src/database.h \
    ../../src/callsign.h \
    ../../src/dataproxy_sqlite.h \
    ../../src/frequency.h \
    ../../src/locator.h \
    ../../src/database/queryexecutor.h \
    ../../src/qso.h \
    ../../src/qsodatacache.h \
    ../../src/utilities.h \
    ../../src/world.h \
    ../../src/filemanager.h

SOURCES +=  tst_filemanager.cpp \
    ../../src/adif.cpp \
    ../../src/database/db_adif_primary_subdvisions_data.cpp \
    ../../src/awards.cpp \
    ../../src/awarddxmarathon.cpp \
    ../../src/database.cpp \
    ../../src/callsign.cpp \
    ../../src/dataproxy_sqlite.cpp \
    ../../src/frequency.cpp \
    ../../src/locator.cpp \
    ../../src/database/queryexecutor.cpp \
    ../../src/qso.cpp \
    ../../src/qsodatacache.cpp \
    ../../src/utilities.cpp \
    ../../src/world.cpp \
    ../../src/filemanager.cpp

isEmpty(QMAKE_LRELEASE) {
    win32|os2:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
    unix {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease-qt5 }
    } else {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease }
    }
}
