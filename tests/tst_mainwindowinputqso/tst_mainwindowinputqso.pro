QT += testlib \
    core \
    gui \
    sql \
    widgets

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    ../../src/inputwidgets/mainwindowinputqso.h \
    ../../src/dataproxy_sqlite.h \
    ../../src/database.h \
    ../../src/database/queryexecutor.h \
    ../../src/database/db_adif_primary_subdvisions_data.h \
    ../../src/frequency.h \
    ../../src/qso.h \
    ../../src/callsign.h \
    ../../src/utilities.h \
    ../../src/locator.h \
    ../../src/adif.h

SOURCES +=  tst_mainwindowinputqso.cpp \
    ../../src/inputwidgets/mainwindowinputqso.cpp \    
    ../../src/dataproxy_sqlite.cpp \
    ../../src/database.cpp \
    ../../src/database/queryexecutor.cpp \
    ../../src/database/db_adif_primary_subdvisions_data.cpp \
    ../../src/frequency.cpp \
    ../../src/qso.cpp \
    ../../src/callsign.cpp \
    ../../src/utilities.cpp \
    ../../src/locator.cpp \
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
