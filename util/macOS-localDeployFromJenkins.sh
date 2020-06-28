#!/bin/bash
# Script to deploy KLog in OSX
# Must be executed in the build directory of KLog
KLOG_VERSION=`grep "VERSION =" KLog.pro |awk '{print $3}'`
echo "Building KLog-$KLOG_VERSION"
export CXXFLAGS=-std=c++11


KLOG_SOURCES="../klog"
QTDIRi=$HOME"/Qt/5.11.1/clang_64"
rm -Rf KLog.app
if [ $? -ne 0 ]; then
echo "There was an error when building KLog-1"
exit 1
fi

rm -Rf build 
if [ $? -ne 0 ]; then
echo "There was an error when building KLog-2"
exit 1
fi

$QTDIRi/bin/qmake KLog.pro -spec macx-clang CONFIG+=x86_64 
if [ $? -ne 0 ]; then
echo "There was an error when building KLog-4"
exit 1
fi

/usr/bin/make
if [ $? -ne 0 ]; then
echo "There was an error when building KLog-5"
exit 1
fi

mv build/target/klog.app KLog.app
if [ $? -ne 0 ]; then
echo "There was an error when building KLog-6"
exit 1
fi

mkdir -p KLog.app/Contents/PlugIns/sqldrivers
if [ $? -ne 0 ]; then
echo "There was an error when building KLog-7"
exit 1
fi

cp $QTDIRi/plugins/sqldrivers/libqsqlite.dylib KLog.app/Contents/PlugIns/sqldrivers
if [ $? -ne 0 ]; then
echo "There was an error when building KLog-8"
exit 1
fi

mkdir -p KLog.app/Contents/MacOS/translations
if [ $? -ne 0 ]; then
echo "There was an error when building KLog-9"
exit 1
fi

$QTDIRi/bin/lupdate KLog.pro
if [ $? -ne 0 ]; then
echo "There was an error when building KLog-10"
exit 1
fi

$QTDIRi/bin/lrelease KLog.pro
if [ $? -ne 0 ]; then
echo "There was an error when building KLog-11"
exit 1
fi

cp $KLOG_SOURCES/translations/*.qm KLog.app/Contents/MacOS/translations/
if [ $? -ne 0 ]; then
echo "There was an error when building KLog-11"
exit 1
fi

cp /usr/local/lib/libhamlib.2.dylib KLog.app/Contents/MacOS/
if [ $? -ne 0 ]; then
echo "There was an error when building KLog-12"
exit 1
fi

install_name_tool -id @executable_path/libhamlib.2.dylib KLog.app/Contents/MacOS/libhamlib.2.dylib
if [ $? -ne 0 ]; then
echo "There was an error when building KLog-13"
exit 1
fi

install_name_tool -change /usr/local/lib/libhamlib.2.dylib @executable_path/libhamlib.2.dylib KLog.app/Contents/MacOS/klog
if [ $? -ne 0 ]; then
echo "There was an error when building KLog-14"
exit 1
fi

$QTDIRi/bin/macdeployqt KLog.app/ -dmg
if [ $? -ne 0 ]; then
echo "There was an error when building KLog-15"
exit 1
fi

mv KLog.dmg KLog-$KLOG_VERSION.dmg
if [ $? -eq 0 ]; then
echo "You can find the dmg file in this folder... enjoy KLog!"
exit 0
else
echo "There was an error when building KLog-16"
exit 1
fi
