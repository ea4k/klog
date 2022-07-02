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
    ../../src/setupdialog.h \
    ../../src/setuppages/hamlibnetworkconfigwidget.h \
    ../../src/setuppages/hamlibserialconfigwidget.h \
    ../../src/setuppages/setupentitydialog.h \
    ../../src/setuppages/setuppagebandmode.h \
    ../../src/setuppages/setuppagecolors.h \
    ../../src/setuppages/setuppagedxcluster.h \
    ../../src/setuppages/setuppageelog.h \
    ../../src/setuppages/setuppagehamlib.h \
    ../../src/setuppages/setuppagelogs.h \
    ../../src/setuppages/setuppagelogsnew.h \
    ../../src/setuppages/setuppagelogview.h \
    ../../src/setuppages/setuppagemisc.h \
    ../../src/setuppages/setuppagesats.h \
    ../../src/setuppages/setuppagesatseditor.h \
    ../../src/setuppages/setuppagesatsnew.h \
    ../../src/setuppages/setuppagesubdivisionnew.h \
    ../../src/setuppages/setuppagesubdivisions.h \
    ../../src/setuppages/setuppageudp.h \
    ../../src/setuppages/setuppageuserdata.h \
    ../../src/setuppages/setuppageworldeditor.h \
    ../../src/aboutdialog.h \
    ../../src/tipsdialog.h \
    ../../src/filemanager.h \
    ../../src/fileawardmanager.h \
    ../../src/locator.h \
    ../../src/dxcluster.h \
    ../../src/awards.h \
    ../../src/awarddxmarathon.h \
    ../../src/inputwidgets/mainwindowsattab.h \
    ../../src/inputwidgets/mainwindowmydatatab.h \
    ../../src/inputwidgets/mainwindowinputcomment.h \
    ../../src/inputwidgets/mainwindowinputothers.h \
    ../../src/inputwidgets/mainwindowinputeqsl.h \
    ../../src/inputwidgets/mainwindowinputqsl.h \
    ../../src/inputwidgets/mainwindowinputqso.h \
    ../../src/mainqsoentrywidget.h \
    ../../src/elogclublog.h \
    ../../src/downloadcty.h \
    ../../src/dxccstatuswidget.h \
    ../../src/awardswidget.h \
    ../../src/softwareupdate.h \
    ../../src/logmodel.h \
    ../../src/logwindow.h \
    ../../src/searchwidget.h \
    ../../src/searchwindow.h \
    ../../src/searchmodel.h \
    ../../src/infowidget.h \
    ../../src/showerrordialog.h \
    ../../src/udpserver.h \
    ../../src/statisticswidget.h \
    ../../src/updatesatsdata.h \
    ../../src/hamlibclass.h \
    ../../src/elogqrzlog.h \
    ../../src/lotwutilities.h \
    ../../src/eqslutilities.h \
    ../../src/widgets/adiflotwexportwidget.h \
    ../../src/widgets/showadifimportwidget.h \
    ../../src/widgets/map/mapwindowwidget.h \
    ../../src/widgets/map/mapwidget.h \
    ../../src/widgets/showkloglogwidget.h \
    ../../src/mainwindow.h \
    ../../src/mainqsoentrywidget.h \
    ../../src/dataproxy_sqlite.h \
    ../../src/database.h \
    ../../src/klogdefinitions.h \
    ../../src/setupdialog.h \
    ../../src/utilities.h \
    ../../src/qso.h \
    ../../src/world.h


SOURCES +=  tst_mainwindow.cpp \
    ../../src/setupdialog.cpp \
    ../../src/aboutdialog.cpp \
    ../../src/tipsdialog.cpp \
    ../../src/filemanager.cpp \
    ../../src/fileawardmanager.cpp \
    ../../src/locator.cpp \
    ../../src/dxcluster.cpp \
    ../../src/awards.cpp \
    ../../src/awarddxmarathon.cpp \
    ../../src/inputwidgets/mainwindowsattab.cpp \
    ../../src/inputwidgets/mainwindowmydatatab.cpp \
    ../../src/inputwidgets/mainwindowinputcomment.cpp \
    ../../src/inputwidgets/mainwindowinputothers.cpp \
    ../../src/inputwidgets/mainwindowinputeqsl.cpp \
    ../../src/inputwidgets/mainwindowinputqsl.cpp \
    ../../src/inputwidgets/mainwindowinputqso.cpp \
    ../../src/mainqsoentrywidget.cpp \
    ../../src/elogclublog.cpp \
    ../../src/downloadcty.cpp \
    ../../src/dxccstatuswidget.cpp \
    ../../src/awardswidget.cpp \
    ../../src/softwareupdate.cpp \
    ../../src/logmodel.cpp \
    ../../src/logwindow.cpp \
    ../../src/searchwidget.cpp \
    ../../src/searchwindow.cpp \
    ../../src/searchmodel.cpp \
    ../../src/infowidget.cpp \
    ../../src/showerrordialog.cpp \
    ../../src/udpserver.cpp \
    ../../src/statisticswidget.cpp \
    ../../src/updatesatsdata.cpp \
    ../../src/hamlibclass.cpp \
    ../../src/elogqrzlog.cpp \
    ../../src/lotwutilities.cpp \
    ../../src/eqslutilities.cpp \
    ../../src/widgets/adiflotwexportwidget.cpp \
    ../../src/widgets/showadifimportwidget.cpp \
    ../../src/widgets/map/mapwindowwidget.cpp \
    ../../src/widgets/map/mapwidget.cpp \
    ../../src/widgets/showkloglogwidget.cpp \
    ../../src/mainwindow.cpp \
    ../../src/mainqsoentrywidget.cpp \
    ../../src/dataproxy_sqlite.cpp \
    ../../src/database.cpp \
    ../../src/setupdialog.cpp \
    ../../src/setuppages/hamlibnetworkconfigwidget.cpp \
    ../../src/setuppages/hamlibserialconfigwidget.cpp \
    ../../src/setuppages/setupentitydialog.cpp \
    ../../src/setuppages/setuppagebandmode.cpp \
    ../../src/setuppages/setuppagecolors.cpp \
    ../../src/setuppages/setuppagedxcluster.cpp \
    ../../src/setuppages/setuppageelog.cpp \
    ../../src/setuppages/setuppagehamlib.cpp \
    ../../src/setuppages/setuppagelogs.cpp \
    ../../src/setuppages/setuppagelogsnew.cpp \
    ../../src/setuppages/setuppagelogview.cpp \
    ../../src/setuppages/setuppagemisc.cpp \
    ../../src/setuppages/setuppagesats.cpp \
    ../../src/setuppages/setuppagesatseditor.cpp \
    ../../src/setuppages/setuppagesatsnew.cpp \
    ../../src/setuppages/setuppagesubdivisionnew.cpp \
    ../../src/setuppages/setuppagesubdivisions.cpp \
    ../../src/setuppages/setuppageudp.cpp \
    ../../src/setuppages/setuppageuserdata.cpp \
    ../../src/setuppages/setuppageworldeditor.cpp \
    ../../src/utilities.cpp \
    ../../src/qso.cpp \
    ../../src/world.cpp

macx: {
    message(macx)
    CONFIG += c++11
    INCLUDEPATH +=/usr/local/include/
    LIBS += -L"/usr/local/lib" -lhamlib
}


isEmpty(QMAKE_LRELEASE) {
    win32|os2:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
    unix {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease-qt5 }
    } else {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease }
    }
}
