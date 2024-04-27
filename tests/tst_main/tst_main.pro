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
CONFIG += no_testcase_installs

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

TEMPLATE = app

HEADERS += ../../src/setupdialog.h \
    ../../src/aboutdialog.h \
    ../../src/adif.h \
    ../../src/awarddxmarathon.h \
    ../../src/awards.h \
    ../../src/awardswidget.h \
    ../../src/charts/statsfieldperbandwidget.h \
    ../../src/charts/statsgeneralchartwidget.h \
    ../../src/charts/statsdxccsonsatswidget.h \
    ../../src/charts/statsqsosperyearbarchartwidget.h \
    ../../src/charts/statsentitiesperyearbarchartwidget.h \
    ../../src/charts/statscqzperyearbarchartwidget.h \
    ../../src/charts/statsqsosperbandbarchartwidget.h \
    ../../src/charts/statsqsospermodebarchartwidget.h \
    ../../src/charts/statsqsosperdxccbarchartwidget.h \
    ../../src/charts/statsqsospercontinentbarchartwidget.h \
    ../../src/charts/statsqsosperhourbarchartwidget.h \
    ../../src/charts/statsqsospermonthbarchartwidget.h \
    ../../src/charts/statsworkedconfirmedpiechartwidget.h \
    ../../src/charts/statsworkedsentpiechartwidget.h \
    ../../src/charts/statsgridsonsatswidget.h \
    ../../src/charts/statssentconfirmedpiechartwidget.h \
    ../../src/database.h \
    ../../src/dataproxy_sqlite.h \
    ../../src/downloadcty.h \
    ../../src/dxcluster/dxcluster.h \
    ../../src/dxcluster/dxclusterassistant.h \
    ../../src/frequency.h \
    ../../src/dxccstatuswidget.h \
    ../../src/elogqrzlog.h \
    ../../src/eqslutilities.h \
    ../../src/global.h \
    ../../src/hamlibclass.h \
    ../../src/inputwidgets/mainwindowinputqso.h \
    ../../src/inputwidgets/mainwindowinputcomment.h \
    ../../src/inputwidgets/mainwindowmydatatab.h \
    ../../src/inputwidgets/mainwindowinputothers.h \
    ../../src/inputwidgets/mainwindowinputeqsl.h \
    ../../src/inputwidgets/mainwindowinputqsl.h \
    ../../src/inputwidgets/mainwindowsattab.h\
    ../../src/klogdefinitions.h \
    ../../src/lotwutilities.h \
    ../../src/mainqsoentrywidget.h \
    ../../src/mainwindow.h \
    ../../src/qso.h \
    ../../src/searchmodel.h \
    ../../src/searchwindow.h \
    ../../src/logwindow.h \
    ../../src/filemanager.h \
    ../../src/fileawardmanager.h \
    ../../src/locator.h \
    ../../src/startwizard.h \
    ../../src/elogclublog.h \
    ../../src/softwareupdate.h \
    ../../src/softwareupdatedialog.h \
    ../../src/utilities.h \
    ../../src/logmodel.h \
    ../../src/searchwidget.h \
    ../../src/infowidget.h \
    ../../src/showerrordialog.h \
    ../../src/statisticswidget.h \
    ../../src/setuppages/setuppagemisc.h \
    ../../src/setuppages/hamlibnetworkconfigwidget.h \
    ../../src/setuppages/hamlibserialconfigwidget.h \
    ../../src/setuppages/setuppagelogview.h \
    ../../src/setuppages/setuppageuserdata.h \
    ../../src/setuppages/setuppagedxcluster.h \
    ../../src/setuppages/setuppagecolors.h \
    ../../src/setuppages/setuppagelogs.h \
    ../../src/setuppages/setuppageworldeditor.h \
    ../../src/setuppages/setuppagesats.h \
    ../../src/setuppages/setuppagesatsnew.h \
    ../../src/setuppages/setuppagehamlib.h \
    ../../src/setuppages/setuppagelogsnew.h \
    ../../src/setuppages/setuppagebandmode.h \
    ../../src/setuppages/setupentitydialog.h \
    ../../src/setuppages/setuppageudp.h \
    ../../src/setuppages/setuppageelog.h \
    ../../src/setuppages/setuppagesubdivisionnew.h \
    ../../src/setuppages/setuppagesubdivisions.h \
    ../../src/tipsdialog.h \
    ../../src/udpserver.h \
    ../../src/updatesatsdata.h \
    ../../src/widgets/map/mapwidget.h \
    ../../src/widgets/map/mapwindowwidget.h \
    ../../src/widgets/adiflotwexportwidget.h \
    ../../src/widgets/showkloglogwidget.h \
    ../../src/widgets/onlinemessagewidget.h \
    ../../src/widgets/showadifimportwidget.h \
    ../../src/updatesettings.h \
    ../../src/world.h

message(Sources)

SOURCES += tst_main.cpp \
    ../../src/main.cpp \
    ../../src/setupdialog.cpp \
    ../../src/aboutdialog.cpp \
    ../../src/adif.cpp \
    ../../src/awarddxmarathon.cpp \
    ../../src/awards.cpp \
    ../../src/awardswidget.cpp \
    ../../src/charts/statsfieldperbandwidget.cpp \
    ../../src/charts/statsgeneralchartwidget.cpp \
    ../../src/charts/statsdxccsonsatswidget.cpp \
    ../../src/charts/statsqsosperyearbarchartwidget.cpp \
    ../../src/charts/statsentitiesperyearbarchartwidget.cpp \
    ../../src/charts/statscqzperyearbarchartwidget.cpp \
    ../../src/charts/statsqsosperbandbarchartwidget.cpp \
    ../../src/charts/statsqsospermodebarchartwidget.cpp \
    ../../src/charts/statsqsosperdxccbarchartwidget.cpp \
    ../../src/charts/statsqsospercontinentbarchartwidget.cpp \
    ../../src/charts/statsqsosperhourbarchartwidget.cpp \
    ../../src/charts/statsqsospermonthbarchartwidget.cpp \
    ../../src/charts/statsworkedconfirmedpiechartwidget.cpp \
    ../../src/charts/statsworkedsentpiechartwidget.cpp \
    ../../src/charts/statsgridsonsatswidget.cpp \
    ../../src/charts/statssentconfirmedpiechartwidget.cpp \
    ../../src/database.cpp \
    ../../src/dataproxy_sqlite.cpp \
    ../../src/downloadcty.cpp \
    ../../src/dxcluster/dxcluster.cpp \
    ../../src/dxcluster/dxclusterassistant.cpp \
    ../../src/dxccstatuswidget.cpp \
    ../../src/frequency.cpp \
    ../../src/elogqrzlog.cpp \
    ../../src/eqslutilities.cpp \
    ../../src/hamlibclass.cpp \
    ../../src/inputwidgets/mainwindowinputqso.cpp \
    ../../src/inputwidgets/mainwindowinputcomment.cpp \
    ../../src/inputwidgets/mainwindowmydatatab.cpp \
    ../../src/inputwidgets/mainwindowinputothers.cpp \
    ../../src/inputwidgets/mainwindowinputeqsl.cpp \
    ../../src/inputwidgets/mainwindowinputqsl.cpp \
    ../../src/inputwidgets/mainwindowsattab.cpp \
    ../../src/lotwutilities.cpp \
    ../../src/mainqsoentrywidget.cpp \
    ../../src/mainwindow.cpp \
    ../../src/qso.cpp \
    ../../src/searchmodel.cpp \
    ../../src/searchwindow.cpp \
    ../../src/logwindow.cpp \
    ../../src/filemanager.cpp \
    ../../src/fileawardmanager.cpp \
    ../../src/locator.cpp \
    ../../src/startwizard.cpp \
    ../../src/elogclublog.cpp \
    ../../src/softwareupdate.cpp \
    ../../src/softwareupdatedialog.cpp \
    ../../src/utilities.cpp \
    ../../src/logmodel.cpp \
    ../../src/searchwidget.cpp \
    ../../src/infowidget.cpp \
    ../../src/showerrordialog.cpp \
    ../../src/statisticswidget.cpp \
    ../../src/setuppages/setuppagemisc.cpp \
    ../../src/setuppages/hamlibnetworkconfigwidget.cpp \
    ../../src/setuppages/hamlibserialconfigwidget.cpp \
    ../../src/setuppages/setuppagelogview.cpp \
    ../../src/setuppages/setuppageuserdata.cpp \
    ../../src/setuppages/setuppagedxcluster.cpp \
    ../../src/setuppages/setuppagecolors.cpp \
    ../../src/setuppages/setuppagelogs.cpp \
    ../../src/setuppages/setuppageworldeditor.cpp \
    ../../src/setuppages/setuppagesats.cpp \
    ../../src/setuppages/setuppagesatsnew.cpp \
    ../../src/setuppages/setuppagehamlib.cpp \
    ../../src/setuppages/setuppagelogsnew.cpp \
    ../../src/setuppages/setuppagebandmode.cpp \
    ../../src/setuppages/setupentitydialog.cpp \
    ../../src/setuppages/setuppageudp.cpp \
    ../../src/setuppages/setuppageelog.cpp \
    ../../src/setuppages/setuppagesubdivisionnew.cpp \
    ../../src/setuppages/setuppagesubdivisions.cpp \
    ../../src/tipsdialog.cpp \
    ../../src/udpserver.cpp \
    ../../src/updatesatsdata.cpp \
    ../../src/widgets/map/mapwidget.cpp \
    ../../src/widgets/map/mapwindowwidget.cpp \
    ../../src/widgets/adiflotwexportwidget.cpp \
    ../../src/widgets/showkloglogwidget.cpp \
    ../../src/widgets/onlinemessagewidget.cpp \
    ../../src/widgets/showadifimportwidget.cpp \
    ../../src/updatesettings.cpp \
    ../../src/world.cpp

message (More...)
RESOURCES += ../../src/klog.qrc
DESTDIR = ../../src/build/target/
OBJECTS_DIR = ../../src/build/obj/
MOC_DIR = ../../src/build/moc/
RCC_DIR = ../../src/build/rcc/
#D_OBJECTS = $(SRC:%.cpp=build/obj/%.o)
#R_OBJECTS = $(SRC:%.cpp=build/obj/%.o)

# Tell Qt Linguist that we use UTF-8 strings in our sources
CODECFORTR = UTF-8
CODECFORSRC = UTF-8
#include(translations/translations.pri)

isEmpty(QMAKE_LRELEASE) {
    win32|os2:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
    unix {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease-qt5 }
    } else {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease }
    }
}

# deploy
DISTFILES += Changelog COPYING \
    qml/mapqmlfile.qml \
    qml/marker.qml

unix:!mac {
    message(unix:!mac)
    DEFINES += APP_LINUX
    CONFIG  += c++11
    #QMAKE_CXXFLAGS += --coverage
    #LIBS += -lgcov
# Translations should be copied in /usr/share/klog/translations
# https://en.wikipedia.org/wiki/Filesystem_Hierarchy_Standard
#    QT += dbus
    isEmpty(PREFIX):PREFIX = /usr
    BINDIR = $$PREFIX/bin
    DATADIR = $$PREFIX/share
    PKGDATADIR = $$DATADIR/klog
    INSTALLS += target
    target.path = $$BINDIR
    translations.path = $$PKGDATADIR/translations
    translations.files += $$DESTDIR/translations/*
    datafiles.path = $$PKGDATADIR
    datafiles.files = $$DISTFILES
    INSTALLS += translations
    INSTALLS += datafiles
    LIBS += -lhamlib
}

macx: {
    message(macx)
    ICON = klog.icns
    TARGET = KLog
    CONFIG += c++11
    #INCLUDEPATH +=../../../hamlib/include/
    #LIBS += -L"../../../hamlib/lib" -lhamlib
    INCLUDEPATH +=/usr/local/include/
    LIBS += -L"/usr/local/lib" -lhamlib
}

win32: {
    message(windows)
    #RC_ICONS = klog.ico
    TARGET = klog
    QMAKE_TARGET_COMPANY = EA4K
    QMAKE_TARGET_DESCRIPTION = Hamradio logging

    contains(QT_ARCH, i386) {
        message("32-bit")
        LIBS += -L"$$PWD/../../libs/win32/hamlib/lib/gcc" -lhamlib
        LIBS += -L"$$PWD/../../libs/win32/hamlib/bin"
        INCLUDEPATH += "$$PWD/../../libs/win32/hamlib/include/"
    } else {
        message("64-bit")
        LIBS += -L"$$PWD/../../../libs/win64/hamlib/lib/gcc" -lhamlib
        LIBS += -L"$$PWD/../../../libs/win64/hamlib/bin"
        INCLUDEPATH += "$$PWD/../../../libs/win64/hamlib/include/"
    }
}
