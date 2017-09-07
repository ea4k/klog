#!/bin/sh
SOURCES="../klog"
TARGET="../svn"
TRANSLATIONS="../klog/translations"
POFILES="../po"

mkdir -p $TARGET

echo "Preparing the sources to be sent to SVN..."

rm -rf $TARGET/*

cp -r $SOURCES/* ./$TARGET

# Cleaning ...
echo "Cleaning and preparing the sources to be packaged"
rm -f $TARGET/klog.pro.user
rm -f $TARGET/translations/klog_template*
rm -f $TARGET/translations/*.qm
rm -f $TARGET/*~
rm -f $TARGET/translations/*~

echo "Updating and releasing translation files"
lupdate -no-obsolete $TARGET/klog.pro

echo "Your sources are ready to be sent to SVN in the $TARGET folder"

