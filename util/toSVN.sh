#!/bin/sh
SVN="../svn" 
SOURCES="../klog" 
TARGET="../target"
TRANSLATIONS="../klog/translations"
POFILES="../svn/po"
QT_SELECT="qt5"

echo "Preparing the sources to be sent to SVN..."

rm -rf $SVN/*

mkdir -p $SVN/src
mkdir -p $SVN/po

cp -r $SOURCES/* $SVN/src

# Cleaning ...
echo "Cleaning and preparing the sources to be packaged"
rm -f $SVN/src/klog.pro.user
rm -f $SVN/src/translations/klog_template*
rm -f $SVN/src/translations/*.qm
rm -f $SVN/src/*~
rm -f $SVN/src/translations/*~

echo "Updating and releasing translation files"

lupdate -no-obsolete $SVN/src/klog.pro

./translations-ts2po.sh


echo "Your sources are ready to be sent to SVN in the $SVN folder"

