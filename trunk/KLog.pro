CONFIG += app_bundle
CONFIG += static
#CONFIG += release
TEMPLATE = app
VERSION = 0.9.2.7
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
    QT *= printsupport
    QT += widgets
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
    showerrordialog.h
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
    showerrordialog.cpp

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
    translations/klog_hr.ts \
    translations/klog_it.ts \
    translations/klog_pl.ts \
    translations/klog_template.ts \
    translations/klog_ja.ts

# deploy
DISTFILES += CHANGES COPYING
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
