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
KLOG_VERSION=$(grep "PKGVERSION =" src.pro | awk '{print $3}')
echo "Packaging KLog-$KLOG_VERSION"
KLOG_SOURCES="../src"
QTDIRi=$HOME"/Qt/6.10.2/macos"

# Cleaning
rm -Rf KLog.app
rm -Rf build 
/usr/bin/make clean

# Compiling
"$QTDIRi"/bin/qmake6 src.pro -spec macx-clang 
/usr/bin/make

# Renaming for distribution
mv build/target/klog.app KLog.app

# Adding the SQlite plugin
#mkdir -p KLog.app/Contents/PlugIns/sqldrivers
#cp "$QTDIRi"/plugins/sqldrivers/libqsqlite.dylib KLog.app/Contents/PlugIns/sqldrivers

# Updating & Adding translations
mkdir -p KLog.app/Contents/MacOS/translations
$QTDIRi/bin/lupdate src.pro
$QTDIRi/bin/lrelease src.pro
cp "$KLOG_SOURCES"/translations/*.qm KLog.app/Contents/MacOS/translations/

# Managing 3 party libraties

## 1. Copy & Prepare libusb
cp /opt/homebrew/opt/libusb/lib/libusb-1.0.0.dylib KLog.app/Contents/MacOS/
chmod +w KLog.app/Contents/MacOS/libusb-1.0.0.dylib
install_name_tool -id @executable_path/libusb-1.0.0.dylib KLog.app/Contents/MacOS/libusb-1.0.0.dylib

# 2. Copy & prepare libhamlib
cp /opt/homebrew/opt/hamlib/lib/libhamlib.4.dylib KLog.app/Contents/MacOS/
chmod +w KLog.app/Contents/MacOS/libhamlib.4.dylib
install_name_tool -id @executable_path/libhamlib.4.dylib KLog.app/Contents/MacOS/libhamlib.4.dylib

# 3. Fix the internal link, inform hamlib where is libusb
install_name_tool -change /opt/homebrew/opt/libusb/lib/libusb-1.0.0.dylib @executable_path/libusb-1.0.0.dylib KLog.app/Contents/MacOS/libhamlib.4.dylib

# 4. Fix the main binary (KLog) to find Hamlib
install_name_tool -change /opt/homebrew/opt/hamlib/lib/libhamlib.4.dylib @executable_path/libhamlib.4.dylib KLog.app/Contents/MacOS/KLog

# Fixing signatures
#echo "Fixing broken signatures (Ad-Hoc)..."
#codesign --force --deep --sign - KLog.app

# Creating DMG. We create it with macdeployqt6 to ensure all the needed files are in the folder
echo "Creating DMG..."
"$QTDIRi"/bin/macdeployqt6 KLog.app/ -qmldir=./qml/ -dmg

# Removing not needed plugins; macdeployqt6 includes more than needed
rm -f KLog.app/Contents/PlugIns/sqldrivers/libqsqlmimer.dylib
rm -f KLog.app/Contents/PlugIns/sqldrivers/libqsqlodbc.dylib
rm -f KLog.app/Contents/PlugIns/sqldrivers/libqsqlpsql.dylib
rm -f KLog.app/Contents/PlugIns/quick/libqtquickcontrols2iosstyleplugin.dylib
# Remove the DMG file as we will build it with hdiutil
rm -f KLog.dmg

# Signing the full folder to prevent Kill 9 error
codesign --force --deep --sign - KLog.app

# Creating the final DMG file
hdiutil create -volname "KLog" -srcfolder KLog.app -ov -format UDZO KLog.dmg
mv KLog.dmg KLog-"$KLOG_VERSION".dmg
echo "You can find the dmg file in this folder... enjoy KLog!"
