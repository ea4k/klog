QT += testlib \
    gui \
    sql \
    widgets

QT += testlib \
    core \
    gui \
    sql \
    widgets




CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    ../../src/adif.h \
    ../../src/inputwidgets/mainwindowsattab.h \
    ../../src/dataproxy_sqlite.h \
    ../../src/qso.h \
    ../../src/utilities.h \
    ../../src/locator.h \
    ../../src/klogdefinitions.h \
    ../../src/database.h

SOURCES +=  tst_mainwindowsattab.cpp \
    ../../src/adif.cpp \
    ../../src/inputwidgets/mainwindowsattab.cpp \
    ../../src/dataproxy_sqlite.cpp \
    ../../src/qso.cpp \
    ../../src/utilities.cpp \
    ../../src/locator.cpp \
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
