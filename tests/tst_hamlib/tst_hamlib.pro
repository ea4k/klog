QT += testlib \
      widgets

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    ../../src/klogdefinitions.h \
    ../../src/callsign.h \
    ../../src/frequency.h \
    ../../src/locator.h \
    ../../src/utilities.h \
    ../../src/hamlibclass.h

SOURCES +=  tst_hamlib.cpp \
    ../../src/callsign.cpp \
    ../../src/frequency.cpp \
    ../../src/locator.cpp \
    ../../src/utilities.cpp \
    ../../src/hamlibclass.cpp


isEmpty(QMAKE_LRELEASE) {
    win32|os2:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
    unix {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease-qt5 }
    } else {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease }
    }
}

unix:!mac {
    message(unix:!mac)
    DEFINES += APP_LINUX
    QMAKE_CXXFLAGS += -fmessage-length=0

    INSTALLS += datafiles
    LIBS += -lhamlib
}

macx: {
    message(macx)
    INCLUDEPATH +=/usr/local/include/
    LIBS += -L"/usr/local/lib" -lhamlib
}

win32: {
    message(windows)
    contains(QT_ARCH, i386) {
        message("32-bit")
        LIBS += -L"$$PWD/../../libs/win32/hamlib/lib/gcc" -lhamlib
        LIBS += -L"$$PWD/../../libs/win32/hamlib/bin"
        INCLUDEPATH += "$$PWD/../../libs/win32/hamlib/include/"
    } else {
        message("64-bit")
        LIBS += -L"$$PWD/../../libs/win64/hamlib/lib/gcc" -lhamlib
        LIBS += -L"$$PWD/../../libs/win64/hamlib/bin"
        INCLUDEPATH += "$$PWD/../../libs/win64/hamlib/include/"
    }
}
