#***************************************************************************
#                         KLog.pro
#						  -------------------
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
TEMPLATE = subdirs
SUBDIRS += src \
    tests
#CONFIG(debug, debug) {
#        SUBDIRS += tests
#}
message (Compiling)
CONFIG += no_testcase_installs
unix:!mac {
   # QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage -O0 -g
   # QMAKE_LDFLAGS += -fprofile-arcs -ftest-coverage -O0 -g
   # LIBS += -lgcov
}
