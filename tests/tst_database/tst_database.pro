QT += testlib \
    gui \
    sql \
    widgets

DEFINES *= KLOG_TESTING
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    ../../src/database.h \
    ../../src/database/queryexecutor.h \
    ../../src/database/db_adif_primary_subdvisions_data.h \
    ../../src/global.h \
    ../../src/adif.h \
    ../../src/locator.h \
    ../../src/world.h \
    ../../src/dataproxy_sqlite.h \
    ../../src/qso.h \
    ../../src/utilities.h


SOURCES +=  tst_database.cpp \
        ../../src/adif.cpp \
        ../../src/locator.cpp \
        ../../src/utilities.cpp \
        ../../src/world.cpp \
        ../../src/dataproxy_sqlite.cpp \
        ../../src/qso.cpp \
        ../../src/database/queryexecutor.cpp \
        ../../src/database/db_adif_primary_subdvisions_data.cpp \
        ../../src/database.cpp

isEmpty(QMAKE_LRELEASE) {
    win32|os2:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
    unix {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease-qt5 }
    } else {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease }
    }
}
