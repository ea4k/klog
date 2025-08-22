#!/bin/bash
# Script to deploy the macOS package
# Must be executed in the build directory of KLog
#***************************************************************************
#			
#                         macOSDeploy.sh
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
export CXXFLAGS=-std=c++11
export PATH=$HOME/Qt/Tools/Ninja:$PATH
#KLOG_VERSION="$1"
KLOG_VERSION=$(grep "PKGVERSION =" src.pro |awk '{print $3}')
echo "Packaging KLog-$KLOG_VERSION"
KLOG_SOURCES="../src"
QTDIRi=$HOME"/Qt"
rm -Rf KLog.app
rm -Rf build 
/usr/bin/make clean
echo "CMAKE phase starting"
"$QTDIRi"/Tools/CMake/CMake.app/Contents/bin/cmake -S . -B build -G "Ninja" -DQt6_DIR=$HOME/Qt/6.6.3/macos/lib/cmake/Qt6
echo "BUILD phase starting"
"$QTDIRi"/Tools/CMake/Cmake.app/Contents/bin/cmake --build build -j 4

echo "You can find the dmg file in this folder... enjoy KLog!"
