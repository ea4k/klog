echo; echo;
echo "You are going to export the PO files from TS files. This will overwrite the current PO files "
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
QT_SELECT="qt5"

lupdate -no-obsolete $SOURCES/klog.pro

for file in $TRANSLATIONS/*.ts
 do
  echo Exporting $file
  NEWNAME=`echo $file |sed 's:^../ts/::'`
  echo $NEWNAME
  lconvert -if ts -of po -i $file -o ${NEWNAME%.ts}.po
  mv ${NEWNAME%.ts}.po $POFILES
 done
mv $POFILES/klog_template.po $POFILES/klog.pot

# Removing the actively TS maintained files:
#rm $POFILES/klog_es.po
#rm $POFILES/klog_ja.po

echo; echo;
echo "Statistics of the po files"
echo

for file2 in $POFILES/*.po
 do
  echo Analizing $file2
  msgfmt -o /dev/null --statistics $file2
done
                                                  

podebconf-report-po -v --notdebconf --package=klog --potfile=klog.pot --withouttranslators --podir=$POFILES --nodeadline --call=debian-i18n@lists.debian.org --noforce --deadline="+10 days"

