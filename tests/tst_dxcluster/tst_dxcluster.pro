#/***************************************************************************
#                          tst_dxcluster.pro  -  description
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
    gui \
    network \
    sql \
    widgets

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    ../../src/adif.h \
    ../../src/awards.h \
    ../../src/awarddxmarathon.h \
    ../../src/callsign.h \
    ../../src/database/datacache.h \
    ../../src/database/db_adif_primary_subdvisions_data.h \
    ../../src/database/database.h \
    ../../src/database/queryexecutor.h \
    ../../src/dataproxy_sqlite.h \
    ../../src/dxcluster/dxcluster.h \
    ../../src/dxcluster/dxspot.h \
    ../../src/filemanager.h \
    ../../src/frequency.h \
    ../../src/klogdefinitions.h \
    ../../src/locator.h \
    ../../src/qso.h \
    ../../src/qsodatacache.h \
    ../../src/utilities.h \
    ../../src/world.h

SOURCES += tst_dxcluster.cpp \
    ../../src/adif.cpp \
    ../../src/awards.cpp \
    ../../src/awarddxmarathon.cpp \
    ../../src/callsign.cpp \
    ../../src/database/datacache.cpp \
    ../../src/database/db_adif_primary_subdvisions_data.cpp \
    ../../src/database/database.cpp \
    ../../src/database/queryexecutor.cpp \
    ../../src/dataproxy_sqlite.cpp \
    ../../src/dxcluster/dxcluster.cpp \
    ../../src/dxcluster/dxspot.cpp \
    ../../src/filemanager.cpp \
    ../../src/frequency.cpp \
    ../../src/locator.cpp \
    ../../src/qso.cpp \
    ../../src/qsodatacache.cpp \
    ../../src/utilities.cpp \
    ../../src/world.cpp

isEmpty(QMAKE_LRELEASE) {
    win32|os2:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
    unix {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease-qt5 }
    } else {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease }
    }
}
