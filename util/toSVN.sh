#!/bin/sh
SOURCES="../klog"
TARGET="../svn"
TRANSLATIONS="../klog/translations"
POFILES="../po"

mkdir -p $TARGET
mkdir -p $TARGET/po
mkdir -p $TARGET/src

echo "Preparing the sources to be sent to SVN..."

rm -rf $TARGET/*
mkdir -p $TARGET/po
mkdir -p $TARGET/src
cp -r $SOURCES/* ./$TARGET/src/

# Cleaning ...
echo "Cleaning and preparing the sources to be packaged"
rm -f $TARGET/src/klog.pro.user
rm -f $TARGET/src/translations/klog_template*
rm -f $TARGET/src/translations/*.qm
rm -f $TARGET/src/*~
rm -f $TARGET/src/translations/*~

echo "Updating and releasing translation files"
lupdate -no-obsolete $TARGET/klog.pro

./translations-ts2po.sh
cp $POFILES/*.po $TARGET/po/
cp $POFILES/*.pot $TARGET/po/

echo "Your sources are ready to be sent to SVN in the $TARGET folder"

