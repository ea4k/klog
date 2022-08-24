QT += testlib \
    core \
    gui \
    sql \
    widgets

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    ../../src/klogdefinitions.h \
    ../../src/utilities.h \
    ../../src/dataproxy_sqlite.h \
    ../../src/database.h \
    ../../src/qso.h \
    ../../src/locator.h \
    ../../src/setuppages/setuppageelog.h


SOURCES +=  tst_setuppageelog.cpp \
    ../../src/utilities.cpp \
    ../../src/dataproxy_sqlite.cpp \
    ../../src/database.cpp \
    ../../src/qso.cpp \
    ../../src/locator.cpp \
    ../../src/setuppages/setuppageelog.cpp

isEmpty(QMAKE_LRELEASE) {
    win32|os2:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
    unix {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease-qt5 }
    } else {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease }
    }
}
