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
    ../../src/awards.h \
    ../../src/adif.h \
    ../../src/awarddxmarathon.h \
    ../../src/callsign.h \
    ../../src/database/datacache.h \
    ../../src/database/queryexecutor.h \
    ../../src/database/db_adif_primary_subdvisions_data.h \
    ../../src/frequency.h \
    ../../src/database.h \
    ../../src/dataproxy_sqlite.h \
    ../../src/filemanager.h \
    ../../src/locator.h \
    ../../src/logmodel.h \
    ../../src/logwindow.h \
    ../../src/qso.h \
    ../../src/qsodatacache.h \
    ../../src/world.h \
    ../../src/utilities.h

SOURCES += tst_logwindow.cpp \
    ../../src/adif.cpp \
    ../../src/awards.cpp \
    ../../src/awarddxmarathon.cpp \
    ../../src/callsign.cpp \
    ../../src/database/datacache.cpp \
    ../../src/database/queryexecutor.cpp \
    ../../src/database/db_adif_primary_subdvisions_data.cpp \
    ../../src/frequency.cpp \
    ../../src/database.cpp \
    ../../src/dataproxy_sqlite.cpp \
    ../../src/filemanager.cpp \
    ../../src/locator.cpp \
    ../../src/logmodel.cpp \
    ../../src/logwindow.cpp \
    ../../src/qso.cpp \
    ../../src/qsodatacache.cpp \
    ../../src/world.cpp \
    ../../src/utilities.cpp


isEmpty(QMAKE_LRELEASE) {
    win32|os2:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
    unix {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease-qt5 }
    } else {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease }
    }
}
