QT += testlib \
      #sql \
      widgets

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \        
    ../../src/utilities.h \
    ../../src/klogdefinitions.h \
    #../../src/dataproxy_sqlite.h \
    #../../src/database.h \
    ../../src/locator.h \
    ../../src/qso.h

SOURCES +=  tst_qso.cpp \
    ../../src/utilities.cpp \
    #../../src/dataproxy_sqlite.cpp \
    #../../src/database.cpp \
    ../../src/locator.cpp \
    ../../src/qso.cpp



isEmpty(QMAKE_LRELEASE) {
    win32|os2:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
    unix {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease-qt5 }
    } else {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease }
    }
}
