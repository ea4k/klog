#!/bin/bash
# Script to deploy package the sources release tar.gz file
# Must be executed in the build directory of KLog
KLOG_VERSION=`grep "VERSION =" src/src.pro |awk '{print $3}'`
echo "Packaging KLog-$KLOG_VERSION"
export CXXFLAGS=-std=c++11

NEW_NAME=klog-$KLOG_VERSION
mv src $NEW_NAME
if [ $? -ne 0 ]; then
echo "There was an error when copying the KLog folder"
exit 1
fi

tar cvzf $NEW_NAME.tar.gz $NEW_NAME
if [ $? -eq 0 ]; then
echo "You can find the tar.gz file in this folder... enjoy KLog!"
exit 0
else
çecho "There was an error when packaging KLog"
exit 1
fi
