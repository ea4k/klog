#***************************************************************************
#                         src.pro
#
#                      -------------------
#    begin                : jan 2021
#    copyright            : (C) 2021 by Jaime Robles
#    email                : jaime@robles.es
# ***************************************************************************
#/*****************************************************************************
# * This file is part of KLog.                                                *
# *                                                                           *
# *    KLog is free software: you can redistribute it and/or modify           *
# *    it under the terms of the GNU General Public License as published by   *
# *    the Free Software Foundation, either version 3 of the License, or      *
# *    (at your option) any later version.                                    *
# *                                                                           *
# *    KLog is distributed in the hope that it will be useful,                *
# *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
# *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
# *    GNU General Public License for more details.                           *
# *                                                                           *
# *    You should have received a copy of the GNU General Public License      *
# *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
# *                                                                           *
# *****************************************************************************/
QT += core gui
QT += qml quick

CONFIG += c++11
CONFIG += app_bundle
CONFIG += static
#CONFIG += console
CONFIG -=depend_includepath
#CONFIG += release
TEMPLATE = app
PKGVERSION = 2.4.2-RC3  # Version shown in the UI, it shows the package information, format: 2.4.2-RC2(-RC2 only in RC releases)
VERSION    = 2.4.2      # Version used for calculation, no RC versios, format "2.4.2"

DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_PKGVERSION=\\\"$$PKGVERSION\\\"

APP_NAME = KLog
DEFINES += APP_NAME="$$APP_NAME"

APP_UNIX_NAME = klog
DEFINES += APP_UNIX_NAME="$$APP_UNIX_NAME"


message(Building $${APP_NAME} $${VERSION})
message(Building package $${APP_NAME} $${APP_PKGVERSION})
message(Qt $$[QT_VERSION] in $$[QT_INSTALL_PREFIX])

#DEFINES *= QT_NO_DEBUG_OUTPUT
DEFINES *= QT_USE_QSTRINGBUILDER
DEFINES *= QT_STRICT_ITERATORS

TARGET = $${APP_UNIX_NAME}

QT += core \
    gui \
    sql \
    network \
    serialport \
    printsupport \
    charts \
    quickwidgets \
    positioning \
    widgets
QT += location

greaterThan(QT_MAJOR_VERSION, 4):greaterThan(QT_MINOR_VERSION, 4)
{
    macx:QMAKE_LFLAGS += -Wl,-rpath,@executable_path/../Frameworks
}

DEFINES += QT_DISABLE_DEPRECATED_UP_TO=0x050F00

HEADERS += setupdialog.h \
    aboutdialog.h \
    adif.h \
    awarddxmarathon.h \
    awards.h \
    awardswidget.h \
    callsign.h \
    charts/statsfieldperbandwidget.h \
    charts/statsgeneralchartwidget.h \
    charts/statsdxccsonsatswidget.h \
    charts/statsqsosperyearbarchartwidget.h \
    charts/statsentitiesperyearbarchartwidget.h \
    charts/statscqzperyearbarchartwidget.h \
    charts/statsqsosperbandbarchartwidget.h \
    charts/statsqsospermodebarchartwidget.h \
    charts/statsqsosperdxccbarchartwidget.h \
    charts/statsqsospercontinentbarchartwidget.h \
    charts/statsqsosperhourbarchartwidget.h \
    charts/statsqsospermonthbarchartwidget.h \
    charts/statsworkedconfirmedpiechartwidget.h \
    charts/statsworkedsentpiechartwidget.h \
    charts/statsgridsonsatswidget.h \
    charts/statssentconfirmedpiechartwidget.h \
    database.h \
    database/db_adif_primary_subdvisions_data.h \
    database/queryexecutor.h \
    dataproxy_sqlite.h \
    downloadcty.h \
    frequency.h \
    dxccstatuswidget.h \
    dxcluster/dxspot.h \
    dxcluster/dxcluster.h \
    dxcluster/dxclusterassistant.h \
    elogqrzlog.h \
    eqslutilities.h \
    global.h \
    hamlibclass.h \
    inputwidgets/mainwindowinputqso.h \
    inputwidgets/mainwindowinputcomment.h \
    inputwidgets/mainwindowmydatatab.h \
    inputwidgets/mainwindowinputothers.h \
    inputwidgets/mainwindowinputeqsl.h \
    inputwidgets/mainwindowinputqsl.h \
    inputwidgets/mainwindowsattab.h\
    klogdefinitions.h \
    lotwutilities.h \
    mainqsoentrywidget.h \
    mainwindow.h \
    qso.h \
    searchmodel.h \
    searchwindow.h \
    #widgets/advancedsearch/advancedsearchmodel.h \
    #widgets/advancedsearch/advancedsearchwidget.h \
    #widgets/advancedsearch/advancedsearchwindow.h \
    logwindow.h \
    filemanager.h \
    fileawardmanager.h \
    locator.h \
    startwizard.h \
    elogclublog.h \
    softwareupdate.h \
    softwareupdatedialog.h \
    utilities.h \
    logmodel.h \
    searchwidget.h \
    infowidget.h \
    showerrordialog.h \
    statisticswidget.h \
    setuppages/setuppagemisc.h \
    setuppages/hamlibnetworkconfigwidget.h \
    setuppages/hamlibserialconfigwidget.h \
    setuppages/setuppagelogview.h \
    setuppages/setuppageuserdata.h \
    setuppages/setuppagedxcluster.h \
    setuppages/setuppagecolors.h \
    setuppages/setuppagelogs.h \
    setuppages/setuppageworldeditor.h \
    setuppages/setuppagesats.h \
    setuppages/setuppagesatsnew.h \
    setuppages/setuppagehamlib.h \
    setuppages/setuppagelogsnew.h \
    setuppages/setuppagebandmode.h \
    setuppages/setupentitydialog.h \
    setuppages/setuppageudp.h \
    setuppages/setuppageelog.h \
    setuppages/setuppagesubdivisionnew.h \
    setuppages/setuppagesubdivisions.h \
    tipsdialog.h \
    udpserver.h \
    updatesatsdata.h \
    widgets/map/mapwidget.h \
    widgets/map/mapwindowwidget.h \
    widgets/adiflotwexportwidget.h \
    widgets/showkloglogwidget.h \
    widgets/onlinemessagewidget.h \
    widgets/showadifimportwidget.h \
    updatesettings.h \
    world.h

message(Sources)

SOURCES += main.cpp \
    aboutdialog.cpp \
    adif.cpp \
    awardswidget.cpp \
    callsign.cpp \
    charts/statsfieldperbandwidget.cpp \
    database/db_adif_primary_subdvisions_data.cpp \
    database/queryexecutor.cpp \
    dxcluster/dxspot.cpp \
    dxcluster/dxcluster.cpp \
    dxcluster/dxclusterassistant.cpp \
    elogqrzlog.cpp \
    eqslutilities.cpp \
    frequency.cpp \
    lotwutilities.cpp \
    mainqsoentrywidget.cpp \
    mainwindow.cpp \
    inputwidgets/mainwindowinputqso.cpp \
    inputwidgets/mainwindowinputcomment.cpp \
    inputwidgets/mainwindowmydatatab.cpp \
    inputwidgets/mainwindowinputothers.cpp \
    inputwidgets/mainwindowinputeqsl.cpp \
    inputwidgets/mainwindowinputqsl.cpp \
    inputwidgets/mainwindowsattab.cpp\
    searchmodel.cpp \
    searchwindow.cpp \
    setupdialog.cpp \
    setuppages/hamlibnetworkconfigwidget.cpp \
    setuppages/hamlibserialconfigwidget.cpp \
    setuppages/setuppageelog.cpp \
    setuppages/setuppagelogview.cpp \
    setuppages/setuppagesubdivisionnew.cpp \
    setuppages/setuppagesubdivisions.cpp \
    widgets/adiflotwexportwidget.cpp \
    #widgets/advancedsearch/advancedsearchmodel.cpp \
    #widgets/advancedsearch/advancedsearchwidget.cpp \
    #widgets/advancedsearch/advancedsearchwindow.cpp \
    widgets/map/mapwidget.cpp \
    widgets/map/mapwindowwidget.cpp \
    widgets/onlinemessagewidget.cpp \
    widgets/showadifimportwidget.cpp \
    widgets/showkloglogwidget.cpp \
    logwindow.cpp \
    filemanager.cpp \
    fileawardmanager.cpp \
    database.cpp \
    dataproxy_sqlite.cpp \
    downloadcty.cpp \    
    locator.cpp \
    qso.cpp \
    awards.cpp \
    setuppages/setuppagemisc.cpp \
    setuppages/setuppageuserdata.cpp \
    setuppages/setuppagedxcluster.cpp \
    setuppages/setuppagecolors.cpp \
    setuppages/setuppagelogs.cpp \
    setuppages/setuppageworldeditor.cpp \
    setuppages/setuppagelogsnew.cpp \
    setuppages/setuppagebandmode.cpp \
    setuppages/setuppageudp.cpp \
    setuppages/setuppagesats.cpp \
    setuppages/setuppagesatsnew.cpp \
    setuppages/setuppagehamlib.cpp \
    setuppages/setupentitydialog.cpp \
    startwizard.cpp \
    awarddxmarathon.cpp \
    elogclublog.cpp \
    softwareupdate.cpp \
    softwareupdatedialog.cpp \
    utilities.cpp \
    dxccstatuswidget.cpp \
    logmodel.cpp \
    searchwidget.cpp \
    infowidget.cpp \
    showerrordialog.cpp \
    udpserver.cpp \
    statisticswidget.cpp \
    updatesatsdata.cpp \
    charts/statsgeneralchartwidget.cpp \
    charts/statsdxccsonsatswidget.cpp \
    charts/statsqsosperyearbarchartwidget.cpp \
    charts/statsentitiesperyearbarchartwidget.cpp \
    charts/statscqzperyearbarchartwidget.cpp \
    charts/statsqsospermodebarchartwidget.cpp \
    charts/statsqsosperdxccbarchartwidget.cpp \
    charts/statsqsospercontinentbarchartwidget.cpp \
    charts/statsqsosperhourbarchartwidget.cpp \
    charts/statsqsospermonthbarchartwidget.cpp \
    charts/statsworkedconfirmedpiechartwidget.cpp \
    charts/statsworkedsentpiechartwidget.cpp \
    charts/statssentconfirmedpiechartwidget.cpp \
    charts/statsqsosperbandbarchartwidget.cpp \
    charts/statsgridsonsatswidget.cpp \
    hamlibclass.cpp \
    tipsdialog.cpp \
    updatesettings.cpp \
    world.cpp


message (Other files)

OTHER_FILES += \
    README-DEVEL \
    TODO \
    tips-for-devel.txt \
    INSTALL.txt \
    INSTALL-linux \
    INSTALL-win.txt \
    Changelog \
    INSTALL-osx.txt \
    NEWS \
    klog.1 \
    COPYING \
    AUTHORS \
    README

message (More...)
RESOURCES += klog.qrc
#DESTDIR = .
#OBJECTS_DIR = .
#MOC_DIR = .
#RCC_DIR = .
DESTDIR = build/target/
OBJECTS_DIR = build/obj/
MOC_DIR = build/moc/
RCC_DIR = build/rcc/
#D_OBJECTS = $(SRC:%.cpp=build/obj/%.o)
#R_OBJECTS = $(SRC:%.cpp=build/obj/%.o)

# Tell Qt Linguist that we use UTF-8 strings in our sources
CODECFORTR = UTF-8
CODECFORSRC = UTF-8
#include(translations/translations.pri)
message (Translations)

TRANSLATIONS = translations/klog_es.ts \
    translations/klog_ca.ts \
    translations/klog_cs.ts \
    translations/klog_da.ts \
    translations/klog_de.ts \
    translations/klog_fi.ts \
    translations/klog_fr.ts \
    translations/klog_hr.ts \
    translations/klog_it.ts \
    translations/klog_ja.ts \
    translations/klog_lv.ts \
    translations/klog_nl.ts \
    translations/klog_pl.ts \
    translations/klog_ru.ts \
    translations/klog_uk.ts

message(End of translations...)

isEmpty(QMAKE_LRELEASE) {
    win32|os2:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
    unix {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease-qt5 }
    } else {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease }
    }
}

updateqm.input = TRANSLATIONS
updateqm.output = $$DESTDIR/translations/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN} -qm $$DESTDIR/translations/${QMAKE_FILE_BASE}.qm
updateqm.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += updateqm

# deploy
DISTFILES += Changelog COPYING \
    qml/mapqmlfile.qml \
    qml/marker.qml

unix:!mac {
    message(unix:!mac)
    DEFINES += APP_LINUX
    #CONFIG  += c++11
    CONFIG  += c++20
    QMAKE_CXXFLAGS += -fmessage-length=0
    #QMAKE_CXXFLAGS += --coverage
    #QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage -O0
    #LIBS += -lgcov
# Translations should be copied in /usr/share/klog/translations
# https://en.wikipedia.org/wiki/Filesystem_Hierarchy_Standard
#    QT += dbus
    #CXXFLAGS += -Wall -Werror -Wno-error=template-id-cdtor
    #CXXFLAGS += -Wtemplate-id-cdtor
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
    RC_ICONS = klog.ico
    TARGET = klog
    QMAKE_TARGET_COMPANY = EA4K
    QMAKE_TARGET_DESCRIPTION = Hamradio logging
    QMAKE_CXXFLAGS += -std=c++20
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
