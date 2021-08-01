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
CONFIG += app_bundle
CONFIG += static
#CONFIG += console
CONFIG -=depend_includepath
#CONFIG += release
TEMPLATE = app
VERSION = 1.8
DEFINES += APP_VERSION="$$VERSION"


APP_NAME = KLog
DEFINES += APP_NAME="$$APP_NAME"

APP_UNIX_NAME = klog
DEFINES += APP_UNIX_NAME="$$APP_UNIX_NAME"

message(Building $${APP_NAME} $${VERSION})
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
    widgets

greaterThan(QT_MAJOR_VERSION, 4):greaterThan(QT_MINOR_VERSION, 4)
{
    macx:QMAKE_LFLAGS += -Wl,-rpath,@executable_path/../Frameworks
}

HEADERS += src/setupdialog.h \
    src/aboutdialog.h \
    src/awardswidget.h \     \
    src/charts/statsfieldperbandwidget.h \
    src/klogdefinitions.h \
    src/widgets/showkloglogwidget.h \
    src/elogqrzlog.h \
    src/eqslutilities.h \
    src/lotwutilities.h \
    src/mainqsoentrywidget.h \
    src/mainwindow.h \
    src/inputwidgets/mainwindowinputqso.h \
    src/inputwidgets/mainwindowinputcomment.h \
    src/inputwidgets/mainwindowmydatatab.h \
    src/inputwidgets/mainwindowinputothers.h \
    src/inputwidgets/mainwindowinputeqsl.h \
    src/inputwidgets/mainwindowinputqsl.h \
    src/inputwidgets/mainwindowsattab.h\
    src/qso.h \
    src/searchmodel.h \
    src/searchwindow.h \
    src/setuppages/setuppageelog.h \
    src/setuppages/setuppagesubdivisionnew.h \
    src/setuppages/setuppagesubdivisions.h \
    src/widgets/adiflotwexportwidget.h \
    #widgets/advancedsearch/advancedsearchmodel.h \
    #widgets/advancedsearch/advancedsearchwidget.h \
    #widgets/advancedsearch/advancedsearchwindow.h \
    src/widgets/onlinemessagewidget.h \
    src/widgets/showadifimportwidget.h \
    src/world.h \
    src/logwindow.h \
    src/filemanager.h \
    src/fileawardmanager.h \
    src/database.h \
    src/locator.h \
    src/dxcluster.h \
    src/awards.h \
    src/startwizard.h \
    src/downloadcty.h \
    src/dataproxy_sqlite.h \
    src/awarddxmarathon.h \
    src/elogclublog.h \
    src/softwareupdate.h \
    src/softwareupdatedialog.h \
    src/utilities.h \
    src/dxccstatuswidget.h \
    src/logmodel.h \
    src/searchwidget.h \
    src/infowidget.h \
    src/showerrordialog.h \
    src/udpserver.h \
    src/statisticswidget.h \
    src/charts/barchartstats.h \
    src/updatesatsdata.h \
    src/hamlibclass.h \
    src/tipsdialog.h \
    src/worldmapwidget.h \
    src/setuppages/setuppagemisc.h \
    src/setuppages/setuppageuserdata.h \
    src/setuppages/setuppagedxcluster.h \
    src/setuppages/setuppagecolors.h \
    src/setuppages/setuppagelogs.h \
    src/setuppages/setuppageworldeditor.h \
    src/setuppages/setuppagesats.h \
    src/setuppages/setuppagesatsnew.h \
    src/setuppages/setuppagehamlib.h \
    src/setuppages/setuppagelogsnew.h \
    src/setuppages/setuppagebandmode.h \
    src/setuppages/setupentitydialog.h \
    src/setuppages/setuppageudp.h \
    src/charts/statsgeneralchartwidget.h \
    src/charts/statsdxccsonsatswidget.h \
    src/charts/statsqsosperyearbarchartwidget.h \
    src/charts/statsentitiesperyearbarchartwidget.h \
    src/charts/statscqzperyearbarchartwidget.h \
    src/charts/statsqsosperbandbarchartwidget.h \
    src/charts/statsqsospermodebarchartwidget.h \
    src/charts/statsqsosperdxccbarchartwidget.h \
    src/charts/statsqsospercontinentbarchartwidget.h \
    src/charts/statsqsosperhourbarchartwidget.h \
    src/charts/statsqsospermonthbarchartwidget.h \
    src/charts/statsworkedconfirmedpiechartwidget.h \
    src/charts/statsworkedsentpiechartwidget.h \
    src/charts/statsgridsonsatswidget.h \
    src/charts/statssentconfirmedpiechartwidget.h

message(Sources)

SOURCES += src/main.cpp \
    src/aboutdialog.cpp \
    src/awardswidget.cpp \
    src/charts/statsfieldperbandwidget.cpp \
    src/elogqrzlog.cpp \
    src/eqslutilities.cpp \
    src/lotwutilities.cpp \
    src/mainqsoentrywidget.cpp \
    src/mainwindow.cpp \
    src/inputwidgets/mainwindowinputqso.cpp \
    src/inputwidgets/mainwindowinputcomment.cpp \
    src/inputwidgets/mainwindowmydatatab.cpp \
    src/inputwidgets/mainwindowinputothers.cpp \
    src/inputwidgets/mainwindowinputeqsl.cpp \
    src/inputwidgets/mainwindowinputqsl.cpp \
    src/inputwidgets/mainwindowsattab.cpp\
    src/qso.cpp \
    src/searchmodel.cpp \
    src/searchwindow.cpp \
    src/setupdialog.cpp \
    src/setuppages/setuppageelog.cpp \
    src/setuppages/setuppagesubdivisionnew.cpp \
    src/setuppages/setuppagesubdivisions.cpp \
    src/widgets/adiflotwexportwidget.cpp \
    #widgets/advancedsearch/advancedsearchmodel.cpp \
    #widgets/advancedsearch/advancedsearchwidget.cpp \
    #widgets/advancedsearch/advancedsearchwindow.cpp \
    src/widgets/onlinemessagewidget.cpp \
    src/widgets/showadifimportwidget.cpp \
    src/widgets/showkloglogwidget.cpp \
    src/world.cpp \
    src/logwindow.cpp \
    src/filemanager.cpp \
    src/fileawardmanager.cpp \
    src/database.cpp \
    src/dataproxy_sqlite.cpp \
    src/downloadcty.cpp \
    src/dxcluster.cpp \
    src/locator.cpp \
    src/awards.cpp \
    src/setuppages/setuppagemisc.cpp \
    src/setuppages/setuppageuserdata.cpp \
    src/setuppages/setuppagedxcluster.cpp \
    src/setuppages/setuppagecolors.cpp \
    src/setuppages/setuppagelogs.cpp \
    src/setuppages/setuppageworldeditor.cpp \
    src/setuppages/setuppagelogsnew.cpp \
    src/setuppages/setuppagebandmode.cpp \
    src/setuppages/setuppageudp.cpp \
    src/setuppages/setuppagesats.cpp \
    src/setuppages/setuppagesatsnew.cpp \
    src/setuppages/setuppagehamlib.cpp \
    src/setuppages/setupentitydialog.cpp \
    src/startwizard.cpp \
    src/awarddxmarathon.cpp \
    src/elogclublog.cpp \
    src/softwareupdate.cpp \
    src/softwareupdatedialog.cpp \
    src/utilities.cpp \
    src/dxccstatuswidget.cpp \
    src/logmodel.cpp \
    src/searchwidget.cpp \
    src/infowidget.cpp \
    src/showerrordialog.cpp \
    src/udpserver.cpp \
    src/statisticswidget.cpp \
    src/charts/barchartstats.cpp \
    src/updatesatsdata.cpp \
    src/charts/statsgeneralchartwidget.cpp \
    src/charts/statsdxccsonsatswidget.cpp \
    src/charts/statsqsosperyearbarchartwidget.cpp \
    src/charts/statsentitiesperyearbarchartwidget.cpp \
    src/charts/statscqzperyearbarchartwidget.cpp \
    src/charts/statsqsospermodebarchartwidget.cpp \
    src/charts/statsqsosperdxccbarchartwidget.cpp \
    src/charts/statsqsospercontinentbarchartwidget.cpp \
    src/charts/statsqsosperhourbarchartwidget.cpp \
    src/charts/statsqsospermonthbarchartwidget.cpp \
    src/charts/statsworkedconfirmedpiechartwidget.cpp \
    src/charts/statsworkedsentpiechartwidget.cpp \
    src/charts/statssentconfirmedpiechartwidget.cpp \
    src/charts/statsqsosperbandbarchartwidget.cpp \
    src/charts/statsgridsonsatswidget.cpp \
    src/hamlibclass.cpp \
    src/tipsdialog.cpp \
    src/worldmapwidget.cpp


message (Other files)

OTHER_FILES += \
    src/README-DEVEL \
    src/TODO \
    src/tips-for-devel.txt \
    src/INSTALL.txt \
    src/INSTALL-linux \
    src/INSTALL-win.txt \
    src/Changelog \
    src/INSTALL-osx.txt \
    src/NEWS \
    src/klog.1 \
    src/COPYING \
    src/AUTHORS \
    src/README

message (More...)
RESOURCES += src/klog.qrc
DESTDIR = src/build/target/
OBJECTS_DIR = src/build/obj/
MOC_DIR = src/build/moc/
RCC_DIR = src/build/rcc/
#D_OBJECTS = $(SRC:%.cpp=build/obj/%.o)
#R_OBJECTS = $(SRC:%.cpp=build/obj/%.o)

# Tell Qt Linguist that we use UTF-8 strings in our sources
CODECFORTR = UTF-8
CODECFORSRC = UTF-8
#include(translations/translations.pri)
message (Translations)

TRANSLATIONS = src/translations/klog_es.ts \
    src/translations/klog_ca.ts \
    src/translations/klog_cs.ts \
    src/translations/klog_da.ts \
    src/translations/klog_de.ts \
    src/translations/klog_fi.ts \
    src/translations/klog_fr.ts \
    src/translations/klog_hr.ts \
    src/translations/klog_it.ts \
    src/translations/klog_pl.ts \
    src/translations/klog_ja.ts

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
DISTFILES += src/Changelog src/COPYING

unix:!mac {
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
    ICON = src/klog.icns
    TARGET = KLog
    CONFIG += c++11
    LIBS += -lgcov
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
    #INCLUDEPATH +=../../../hamlib/include/
    #LIBS += -L"../../../hamlib/lib" -lhamlib
    INCLUDEPATH +=/usr/local/include/
    LIBS += -L"/usr/local/lib" -lhamlib
}
win32: {
    RC_ICONS = src/klog.ico
    TARGET = klog
    QMAKE_TARGET_COMPANY = EA4K
    QMAKE_TARGET_DESCRIPTION = Hamradio logging
    LIBS += -L"$$PWD/../../libs/hamlib/lib/gcc" -lhamlib
    INCLUDEPATH += "$$PWD/../../libs/hamlib/include/"
    #LIBS += -L"$$PWD/../../libs/hamlib-w32-4.0rc2/lib/gcc" -lhamlib
    #INCLUDEPATH += "$$PWD/../../libs/hamlib-w32-4.0rc2/include/"
}

else:
{
    TARGET = klog
}

