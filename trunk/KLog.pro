CONFIG += app_bundle
CONFIG += static
#CONFIG += release
TEMPLATE = app
VERSION = 0.9.5
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
    widgets

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += printsupport
    QT += widgets \
        charts
}


HEADERS += setupdialog.h \      
    aboutdialog.h \
    mainwindow.h \
    world.h \
    logwindow.h \
    filemanager.h \
    contest.h \
    database.h \
    setuppagemisc.h \
    locator.h \
    dxcluster.h \
    awards.h \   
    setuppageuserdata.h \
    setuppagedxcluster.h \
    setuppagecolors.h \   
    contest_cqwwdxssb.h \ 
    setuppagelogs.h \
    setuppageworldeditor.h \
    setupentitydialog.h \   
    startwizard.h \
    downloadcty.h \
    dataproxy.h \  
    dataproxy_sqlite.h \
    mainwindowsattab.h \
    awarddxmarathon.h \ 
    setuppagelogsnew.h \
    setuppageclublog.h \
    elogclublog.h \
    softwareupdate.h \
    softwareupdatedialog.h \
    utilities.h \  
    dxccstatuswidget.h \
    mainwindowmydatatab.h \
    mainwindowinputcomment.h \
    mainwindowinputothers.h \   
    mainwindowinputeqsl.h \
    mainwindowinputqsl.h \
    setuppagebandmode.h \
    logmodel.h \
    searchwidget.h \
    infowidget.h \
    showerrordialog.h \
    udpserver.h \
    setuppageudp.h \
    statisticswidget.h \
    charts/barchartstats.h \
    charts/donutchartstats.h
SOURCES += main.cpp \
    aboutdialog.cpp \
    setupdialog.cpp \
    mainwindow.cpp \ 
    world.cpp \
    logwindow.cpp \
    filemanager.cpp \
    contest.cpp \
    contest_cqwwdxssb.cpp \
    database.cpp \
    dataproxy.cpp \
    dataproxy_sqlite.cpp \
    downloadcty.cpp \
    dxcluster.cpp \  
    setuppagemisc.cpp \
    locator.cpp \
    awards.cpp \ 
    setuppageuserdata.cpp \
    setuppagedxcluster.cpp \
    setuppagecolors.cpp \   
    setuppagelogs.cpp \     
    setuppageworldeditor.cpp \
    setupentitydialog.cpp \   
    startwizard.cpp \
    mainwindowsattab.cpp \
    awarddxmarathon.cpp \ 
    setuppagelogsnew.cpp \
    setuppageclublog.cpp \
    elogclublog.cpp \
    softwareupdate.cpp \
    softwareupdatedialog.cpp \
    utilities.cpp \  
    dxccstatuswidget.cpp \
    mainwindowmydatatab.cpp \
    mainwindowinputcomment.cpp \
    mainwindowinputothers.cpp \   
    mainwindowinputeqsl.cpp \
    mainwindowinputqsl.cpp \
    setuppagebandmode.cpp \
    logmodel.cpp \
    searchwidget.cpp \
    infowidget.cpp \
    showerrordialog.cpp \
    udpserver.cpp \
    setuppageudp.cpp \
    statisticswidget.cpp \
    charts/barchartstats.cpp \
    charts/donutchartstats.cpp

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


RESOURCES += klog.qrc
DESTDIR = build/target/
OBJECTS_DIR = build/obj/
MOC_DIR = build/moc/
RCC_DIR = build/rcc/

# Tell Qt Linguist that we use UTF-8 strings in our sources
CODECFORTR = UTF-8
CODECFORSRC = UTF-8
#include(translations/translations.pri)
TRANSLATIONS = translations/klog_es.ts \
    translations/klog_ca.ts \
    translations/klog_da.ts \
    translations/klog_fi.ts \
    translations/klog_hr.ts \
    translations/klog_it.ts \
    translations/klog_pl.ts \
    translations/klog_ja.ts

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
DISTFILES += Changelog COPYING
unix:!mac {
    DEFINES += APP_LINUX
    CONFIG   += c++11

#    QT += dbus
    isEmpty(PREFIX):PREFIX = /usr
    BINDIR = $$PREFIX/bin
    INSTALLS += target
    target.path = $$BINDIR
    DATADIR = $$PREFIX/share
    PKGDATADIR = $$DATADIR/klog
    DEFINES += DATADIR=\\\"$$DATADIR\\\" \
        PKGDATADIR=\\\"$$PKGDATADIR\\\"
    translations.path = $$PKGDATADIR
    translations.files += $$DESTDIR/translations

    INSTALLS += translations
}

macx: {
   ICON = klog.icns
   TARGET = KLog
}

win32: {
 RC_ICONS = klog.ico   
 TARGET = klog   
}
else:TARGET = klog
