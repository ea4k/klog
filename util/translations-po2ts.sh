#!/bin/sh
echo; echo;
echo "You are going to import the PO files into TS files. This will overwrite the current TS files "
while true; do
 read -p "Do you want to continue? (y/n) " yn
  case $yn in
   [Yy]* )
   echo ; break;;
   [Nn]* ) exit;;
    * ) echo "Please answer yes or no.";;
  esac
done
SVN="../svn" 
SOURCES="../klog" 
TARGET="../target"
TRANSLATIONS="../klog/translations"
POFILES="../po"

echo "ja: "; msgfmt -o /dev/null --statistics $POFILES/klog_ja.po
echo "hr: "; msgfmt -o /dev/null --statistics $POFILES/klog_hr.po
echo "ca: "; msgfmt -o /dev/null --statistics $POFILES/klog_ca.po
echo "es: "; msgfmt -o /dev/null --statistics $POFILES/klog_es.po
echo "pl: "; msgfmt -o /dev/null --statistics $POFILES/klog_pl.po
echo "it: "; msgfmt -o /dev/null --statistics $POFILES/klog_it.po

#lconvert -if po -of ts -i $POFILES/klog_ja.ts -o $TRANSLATIONS/klog_ja.po
echo; echo;
while true; do
 read -p "If no errors have been shown, do you want to import the po files into the main sources? (y/n) " yn
  case $yn in
   [Yy]* )
    for file in $POFILES/*.po
    do
    	echo Importing $file
	#echo "../po/klog_es.po" | sed 's:^\.\./po/::'
    	NEWNAME=`echo $file |sed 's:^../po/::' `
    	echo $NEWNAME
	lconvert -if po -of ts -i $file -o $TRANSLATIONS/${NEWNAME%.po}.ts
   done
   echo ; break;;
   [Nn]* ) exit;;
    * ) echo "Please answer yes or no.";;
  esac
done
 