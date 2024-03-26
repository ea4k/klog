QT += testlib \
    core \
    gui \
    sql \
    network \
    serialport \
    printsupport \
    charts \
    quickwidgets \
    positioning \
    widgets

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    ../../src/adif.h \
    ../../src/downloadcty.h \
    ../../src/locator.h \
    ../../src/utilities.h \
    ../../src/startwizard.h


SOURCES +=  tst_wizard.cpp \
    ../../src/adif.cpp \
    ../../src/downloadcty.cpp \
    ../../src/locator.cpp \
    ../../src/utilities.cpp \
    ../../src/startwizard.cpp



isEmpty(QMAKE_LRELEASE) {
    win32|os2:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
    unix {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease-qt5 }
    } else {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease }
    }
}
