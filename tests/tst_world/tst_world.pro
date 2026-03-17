QT += testlib \
    gui \
    sql \
    widgets

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app


HEADERS += \
    ../../src/world.h \
    ../../src/callsign.h \
    ../../src/frequency.h \
    ../../src/qso.h \
    ../../src/global.h \
    ../../src/klogdefinitions.h \
    ../../src/locator.h \
    ../../src/dataproxy_sqlite.h \
    ../../src/utilities.h \
    ../../src/database/database.h \
    ../../src/database/datacache.h \
    ../../src/database/queryexecutor.h \
    ../../src/database/db_adif_primary_subdvisions_data.h \
    ../../src/adif.h

SOURCES +=  tst_world.cpp \
    ../../src/world.cpp \
    ../../src/frequency.cpp \
    ../../src/qso.cpp \
    ../../src/callsign.cpp \
    ../../src/locator.cpp \
    ../../src/dataproxy_sqlite.cpp \
    ../../src/utilities.cpp \
    ../../src/database/database.cpp \
    ../../src/database/datacache.cpp \
    ../../src/database/queryexecutor.cpp \
    ../../src/database/db_adif_primary_subdvisions_data.cpp \
    ../../src/adif.cpp

isEmpty(QMAKE_LRELEASE) {
    win32|os2:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
    unix {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease-qt5 }
    } else {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease }
    }
}
