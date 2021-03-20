QT += testlib \
    gui \
    sql \
    widgets

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    ../../src/dataproxy_sqlite.h \
    ../../src/utilities.h \
    ../../src/locator.h \
    ../../src/database.h \
    ../../src/qso.h \
    ../../src/klogdefinitions.h

SOURCES +=  tst_dataproxy.cpp \
        ../../src/utilities.cpp \
        ../../src/locator.cpp \
        ../../src/database.cpp \
        ../../src/qso.cpp \
        ../../src/dataproxy_sqlite.cpp



isEmpty(QMAKE_LRELEASE) {
    win32|os2:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
    unix {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease-qt5 }
    } else {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease }
    }
}
