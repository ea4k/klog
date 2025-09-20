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
#KLOG_VERSION=$(grep "PKGVERSION =" src.pro |awk '{print $3}')
KLOG_VERSION=$(grep "project(KLog VERSION" ../CMakeLists.txt |awk '{print $3}')
echo "KLOG Packaging KLog-$KLOG_VERSION"
cd ..
KLOG_SOURCES="../src"
QTDIRi=$HOME"/Qt"
QTDIRbin=$HOME"/Qt/6.7.3/macos/bin/"
echo "KLOG CLEANING and preparing to the new build"
rm -Rf KLog.app
rm -Rf build 
/usr/bin/make clean
echo "KLOG CMAKE phase starting"
"$QTDIRi"/Tools/CMake/CMake.app/Contents/bin/cmake -S . -B build -G "Ninja" -DQt6_DIR=$HOME/Qt/6.6.3/macos/lib/cmake/Qt6
echo "KLOG BUILD phase starting"
"$QTDIRi"/Tools/CMake/Cmake.app/Contents/bin/cmake --build build -j 4
echo "KLOG BUILD done"
mkdir KLog.app
mkdir -p KLog.app/Contents/PlugIns/sqldrivers
cp build/bin/klog KLog.app/Contents/MacOS/KLog
cp "$QTDIRi"/plugins/sqldrivers/libqsqlite.dylib KLog.app/Contents/PlugIns/sqldrivers
mkdir -p KLog.app/Contents/MacOS/translations
#$QTDIRi/bin/lupdate src.pro
#$QTDIRi/bin/lrelease src.pro
#cp "$KLOG_SOURCES"/translations/*.qm KLog.app/Contents/MacOS/translations/
cp /usr/local/lib/libhamlib.4.dylib KLog.app/Contents/MacOS/
chmod +w KLog.app/Contents/MacOS/libhamlib.4.dylib
install_name_tool -id @executable_path/libhamlib.4.dylib KLog.app/Contents/MacOS/libhamlib.4.dylib
install_name_tool -change /usr/local/lib/libhamlib.4.dylib @executable_path/libhamlib.4.dylib KLog.app/Contents/MacOS/klog
"$QTDIRbin"macdeployqt6 KLog.app/ -qmldir=src/qml/ -dmg
mv KLog.dmg KLog-"$KLOG_VERSION".dmg
echo "You can find the dmg file in this folder... enjoy KLog!"
