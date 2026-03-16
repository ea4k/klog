#/***************************************************************************
#                          tests.pro  -  description
#                             -------------------
#    begin                : Jan 2021
#    copyright            : (C) 2021 by Jaime Robles
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

#QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage -O0
#LIBS += -lgcov

TEMPLATE=subdirs
SUBDIRS=\
   tst_dxcluster \
   tst_wizard \
   tst_adif \
   tst_callsign \
   tst_dxspot \
   tst_frequency \
   tst_utilities \
   tst_mainqsoentrywidget \
   tst_mainwindowsattab \
   tst_dataproxy \
   tst_database \
   tst_datacache \
   tst_dxcluster \
   tst_dxspot \
   tst_elogclublog \
   tst_filemanager \
   tst_hamlib \
   tst_locator \
   tst_logwindow \
   tst_mainqsoentrywidget \
   tst_mainwindow \
   tst_mainwindowinputqso \
   tst_mainwindowinputothers \
   tst_mainwindowsattab \
   tst_qso \
   tst_setuphamlibnetworkwidget \
   tst_setuphamlibserialwidget \
   tst_setuppageelog \
   tst_world
