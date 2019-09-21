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
KLOG_SOURCES="../klog"
QTDIRi=$HOME"/Qt/5.11.1/clang_64"
rm -R KLog.app
rm -R build 
/usr/bin/make clean
$QTDIRi/bin/qmake KLog.pro -spec macx-clang CONFIG+=x86_64 
/usr/bin/make
mv build/target/klog.app KLog.app
mkdir -p KLog.app/Contents/PlugIns/sqldrivers
cp $QTDIRi/plugins/sqldrivers/libqsqlite.dylib KLog.app/Contents/PlugIns/sqldrivers
mkdir -p KLog.app/Contents/MacOS/translations
$QTDIRi/bin/lupdate KLog.pro
$QTDIRi/bin/lrelease KLog.pro
cp $KLOG_SOURCES/translations/*.qm KLog.app/Contents/MacOS/translations/
install_name_tool -change /usr/local/lib/libhamlib.2.dylib  @executable_path/../Frameworks/libhamlib.2.dylib KLog.app/Contents/MacOS/klog
$QTDIRi/bin/macdeployqt KLog.app/ -dmg
mv KLog.dmg KLog-$KLOG_VERSION.dmg
echo "You can find the dmg file in this folder... enjoy KLog!"
