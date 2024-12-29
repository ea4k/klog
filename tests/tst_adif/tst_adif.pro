QT += testlib \
      widgets

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

#QMAKE_CXXFLAGS += -Wall -Wextra -Werror
#QMAKE_CFLAGS += -Wall -Wextra -Werror

HEADERS += \
    ../../src/adif.h \
    ../../src/callsign.h \
    ../../src/klogdefinitions.h

SOURCES +=  tst_adif.cpp \
    ../../src/callsign.cpp \
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
