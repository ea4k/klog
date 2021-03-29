#!/bin/bash
# Script to deploy KLog in OSX
# Must be executed in the build directory of KLog
if [ $# -ne 1 ]; then
	echo "You must include the version of KLog you want to deploy as a single parameter"
	echo ""
	echo "example: deployOSX.sh 0.9.2 "
	echo ""
	echo ""
	exit -1
 fi
export CXXFLAGS=-std=c++11
KLOG_VERSION="$1"
KLOG_SOURCES="../src"
QTDIRi=$HOME"/Qt/5.15.2/clang_64"
rm -Rf KLog.app
rm -Rf build 
/usr/bin/make clean
$QTDIRi/bin/qmake src.pro -spec macx-clang CONFIG+=x86_64 
/usr/bin/make
mv build/target/klog.app KLog.app
mkdir -p KLog.app/Contents/PlugIns/sqldrivers
cp $QTDIRi/plugins/sqldrivers/libqsqlite.dylib KLog.app/Contents/PlugIns/sqldrivers
mkdir -p KLog.app/Contents/MacOS/translations
$QTDIRi/bin/lupdate src.pro
$QTDIRi/bin/lrelease src.pro
cp $KLOG_SOURCES/translations/*.qm KLog.app/Contents/MacOS/translations/
cp /usr/local/lib/libhamlib.4.dylib KLog.app/Contents/MacOS/
chmod +w KLog.app/Contents/MacOS/libhamlib.4.dylib
install_name_tool -id @executable_path/libhamlib.4.dylib KLog.app/Contents/MacOS/libhamlib.4.dylib
install_name_tool -change /usr/local/lib/libhamlib.4.dylib @executable_path/libhamlib.4.dylib KLog.app/Contents/MacOS/klog
$QTDIRi/bin/macdeployqt KLog.app/ -dmg
mv KLog.dmg KLog-$KLOG_VERSION.dmg
echo "You can find the dmg file in this folder... enjoy KLog!"
