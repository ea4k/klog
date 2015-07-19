# -------------------------------------------------
# Project created by QtCreator 2011-04-23T14:06:58
# -------------------------------------------------
QT += core \
    gui \
    sql \
    network
greaterThan(QT_MAJOR_VERSION, 4) { 
    QT *= printsupport
    QT += widgets
}
unix { 
    macx:TARGET = KLog
    
    # QMAKE_LFLAGS += -F/Library/Frameworks
    # LIBS += -framework TheFramework
    # SOURCES += windowinfo_mac.cpp
    else:# SOURCES += windowinfo_x11.cpp
    TARGET = klog
}
else:TARGET = klog

# greaterThan(QT_MAJOR_VERSION, 4): QT += widgets core gui sql network
# TARGET = klog
CONFIG += app_bundle
CONFIG += static
TEMPLATE = app
SOURCES += main.cpp \
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
    setuppagebandsmodes.cpp \
    setuppagemisc.cpp \
    locator.cpp \
    awards.cpp \
    setuppageuserdata.cpp \
    setuppagedxcluster.cpp \
    setuppagecolors.cpp \
    setuppagelogs.cpp \
    helphelpdialog.cpp \
    helpaboutdialog.cpp \
    setuppageworldeditor.cpp \
    setupentitydialog.cpp \
    startwizard.cpp \
    mainwindowsattab.cpp \
    awarddxmarathon.cpp \
    setuppagelogsnew.cpp \
    setuppageclublog.cpp \
    elogclublog.cpp \
    utilities.cpp
HEADERS += setupdialog.h \
    mainwindow.h \
    world.h \
    logwindow.h \
    filemanager.h \
    contest.h \
    database.h \
    setuppagebandsmodes.h \
    setuppagemisc.h \
    locator.h \
    dxcluster.h \
    awards.h \
    setuppageuserdata.h \
    setuppagedxcluster.h \
    setuppagecolors.h \
    contest_cqwwdxssb.h \
    setuppagelogs.h \
    helphelpdialog.h \
    helpaboutdialog.h \
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
    utilities.h
OTHER_FILES += TODO \
    README-DEVEL \
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
TRANSLATIONS = klog_es.ts \
    klog_fr.ts
CODECFORTR = UTF-8
FORMS += 
