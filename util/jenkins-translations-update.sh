echo; echo;
echo "dYou are going to export the PO files from TS files. This will overwrite the current PO files "

SOURCES="trunk"
TRANSLATIONS="trunk/translations"
POFILES="po"
QT_SELECT="qt5"

#mkdir -p $POFILES

sed 's/translations\/klog_ja.ts/translations\/klog_ja.ts translations\/klog_template.ts/' $SOURCES/KLog.pro >$SOURCES/KLogTemp.pro

lupdate -no-obsolete $SOURCES/KLogTemp.pro

for file in $TRANSLATIONS/*.ts
 do
  echo Exporting $file
  NEWNAME=`echo $file |sed 's:^../ts/::'`
  echo $NEWNAME
  lconvert -if ts -of po -i $file -o ${NEWNAME%.ts}.po
  mv ${NEWNAME%.ts}.po $POFILES
 done

mv $POFILES/klog_template.po $POFILES/klog.pot

echo; echo;
echo "Statistics of the po files"
echo

for file2 in $POFILES/*.po
 do
  echo Analizing $file2
  msgfmt -o /dev/null --statistics $file2
done
                                                  



#podebconf-report-po -v --notdebconf --package=klog --potfile=klog.pot --podir=$POFILES --nodeadline --call=debian-i18n@lists.debian.org --noforce --deadline="+10 days"

