#!/bin/sh
SOURCES="../klog"
TARGET="../target"
TRANSLATIONS="../klog/translations"
POFILES="../po"

mkdir -p $TARGET

echo "Getting the version to be released..."
VERSION=`grep "QString version" $SOURCES/main.cpp | awk '{print $4}' | sed 's:^.\(.*\).$:\1:' `
echo "Version: $VERSION to be released!"

rm -rf $TARGET/klog-$VERSION

cp -r $SOURCES ./$TARGET/klog-$VERSION

# Cleaning ...
echo "Cleaning and preparing the sources to be packaged"
rm -f $TARGET/klog-$VERSION/klog.pro.user
rm -f $TARGET/klog-$VERSION/translations/klog_template*
rm -f $TARGET/klog-$VERSION/translations/*.qm
rm -f $TARGET/*~
rm -f $TARGET/translations/*~

sed -i '/klog_template/d' $TARGET/klog-$VERSION/klog.pro

echo "Updating and releasing translation files"
lupdate -no-obsolete $TARGET/klog-$VERSION/klog.pro
#lrelease klog-$VERSION/klog.pro
cd $TARGET
tar cvzf klog-$VERSION.tar.gz klog-$VERSION
#tar cvzf $TARGET/klog-$VERSION.tar.gz $TARGET/klog-$VERSION
rm -rf $TARGET//klog-$VERSION

echo "You can find the klog-$VERSION package ready for release on the $TARGET folder"

