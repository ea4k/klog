#/***************************************************************************
#                          tst_elogclublog.pro  -  description
#                             -------------------
#    begin                : Apr 2026
#    copyright            : (C) 2026 by Jaime Robles
#    email                : jaime@robles.es
# ***************************************************************************/
#
#/****************************************************************************
# * This file is part of Testing suite of KLog.                               *
# *                                                                           *
# *    KLog is free software: you can redistribute it and/or modify           *
# *    it under the terms of the GNU General Public License as published by   *
# *    the Free Software Foundation, either version 3 of the License, or      *
# *    (at your option) any later version.                                    *
# *                                                                           *
# *    The testing suite of KLog is distributed in the hope that it will      *
# *    be useful, but WITHOUT ANY WARRANTY; without even the implied          *
# *    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.       *
# *    See the GNU General Public License for more details.                   *
# *                                                                           *
# *    You should have received a copy of the GNU General Public License      *
# *    along with Testing suite of KLog.                                      *
# *    If not, see <https://www.gnu.org/licenses/>.                           *
# *                                                                           *
# *****************************************************************************/
QT += testlib \
    core \
    sql \
    network \
    widgets

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    ../../src/elog/elogclublog.h \
    ../../src/adif.h \
    ../../src/callsign.h \
    ../../src/locator.h \
    ../../src/utilities.h \
    ../../src/database/database.h \
    ../../src/database/queryexecutor.h \
    ../../src/database/db_adif_primary_subdvisions_data.h \
    ../../src/klogdefinitions.h

SOURCES += tst_elogclublog.cpp \
    ../../src/elog/elogclublog.cpp \
    ../../src/adif.cpp \
    ../../src/callsign.cpp \
    ../../src/locator.cpp \
    ../../src/database/database.cpp \
    ../../src/database/queryexecutor.cpp \
    ../../src/database/db_adif_primary_subdvisions_data.cpp \
    ../../src/utilities.cpp

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
}

macx: {
    message(macx)
}

win32: {
    message(windows)
}

