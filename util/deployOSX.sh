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
KLOG_VERSION="$1"
KLOG_SOURCES="../klog"
QTDIRi=$HOME"/Qt5.5.1/5.5/clang_64"
rm -R KLog.app
rm -R build 
/usr/bin/make clean
$QTDIRi/bin/qmake KLog.pro
/usr/bin/make
mv build/target/klog.app KLog.app
mkdir -p KLog.app/Contents/PlugIns/sqldrivers
cp $QTDIRi/plugins/sqldrivers/libqsqlite.dylib KLog.app/Contents/PlugIns/sqldrivers
mkdir -p KLog.app/Contents/MacOS/translations
$QTDIRi/bin/lupdate KLog.pro
$QTDIRi/bin/lrelease KLog.pro
cp $KLOG_SOURCES/translations/*.qm KLog.app/Contents/MacOS/translations/
$QTDIRi/bin/macdeployqt KLog.app/ -dmg
mv KLog.dmg KLog-$KLOG_VERSION.dmg
echo "You can find the dmg file in this folder... enjoy KLog!"
