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

# --coverage option is synonym for: -fprofile-arcs -ftest-coverage -lgcov
INCLUDEPATH +=/usr/local/Cellar/llvm/12.0.1/include/
#LIBS += -L"/usr/local/lib" -lhamlib
LIBS += -L"/usr/local/Cellar/llvm/12.0.1/lib" -lgcov
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage

include( $$PWD/src/src.pri )
include( $$PWD/tests/tests.pri )
