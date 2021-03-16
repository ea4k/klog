SOURCES="src"
TRANSLATIONS="src/translations"
POFILES="po"
QT_SELECT="qt5"
lupdate -no-obsolete $SOURCES/src.pro


for file in $TRANSLATIONS/*.ts
 do
  echo Exporting $file
  NEWNAME=`echo $file |sed 's:^../ts/::'`
  echo $NEWNAME
  lconvert -if ts -of po -i $file -o ${NEWNAME%.ts}.po
  mv ${NEWNAME%.ts}.po $POFILES
 done

mv $POFILES/klog_en.po $POFILES/klog.pot

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
                                                  



#podebconf-report-po -v --notdebconf --package=klog --potfile=klog.pot --podir=$POFILES --nodeadline --call=debian-i18n@lists.debian.org --noforce --deadline="+10 days"

