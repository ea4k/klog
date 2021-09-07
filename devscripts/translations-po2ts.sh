#!/bin/sh
# Script to convert po files into ts
# Must be executed in the build directory of KLog
#***************************************************************************
#			
#                         translations-po2ts.sh
#						  -------------------
#    begin                : jan 2021
#    copyright            : (C) 2021 by Jaime Robles
#    email                : jaime@robles.es
# ***************************************************************************
#/*****************************************************************************
# * This file is part of KLog.                                                *
# *                                                                           *
# *    KLog is free software: you can redistribute it and/or modify           *
# *    it under the terms of the GNU General Public License as published by   *
# *    the Free Software Foundation, either version 3 of the License, or      *
# *    (at your option) any later version.                                    *
# *                                                                           *
# *    KLog is distributed in the hope that it will be useful,                *
# *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
# *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
# *    GNU General Public License for more details.                           *
# *                                                                           *
# *    You should have received a copy of the GNU General Public License      *
# *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
# *                                                                           *
# *****************************************************************************/
#SOURCES="src" 
TRANSLATIONS="src/translations"
POFILES="po"
export QT_SELECT="qt5"


for file in $POFILES/*.po
 do
  echo Checking "$file"
  NEWNAME=echo "$("$file" |sed 's:^../po/::')"
  #NEWNAME=`echo "$file" |sed 's:^../po/::'`
  echo $NEWNAME
  msgfmt -o /dev/null --statistics $file
  
#  lconvert -if ts -of po -i $file -o ${NEWNAME%.ts}.po
#  mv ${NEWNAME%.ts}.po $POFILES
 done



echo "ja: "; msgfmt -o /dev/null --statistics $POFILES/klog_ja.po
#echo "ca: "; msgfmt -o /dev/null --statistics $POFILES/klog_ca.po
#echo "da: "; msgfmt -o /dev/null --statistics $POFILES/klog_da.po
#echo "de: "; msgfmt -o /dev/null --statistics $POFILES/klog_de.po
#echo "fi: "; msgfmt -o /dev/null --statistics $POFILES/klog_fi.po
#echo "fr: "; msgfmt -o /dev/null --statistics $POFILES/klog_fr.po
#echo "fr: "; msgfmt -o /dev/null --statistics $POFILES/klog_hr.po
#echo "es: "; msgfmt -o /dev/null --statistics $POFILES/klog_es.po
#echo "pl: "; msgfmt -o /dev/null --statistics $POFILES/klog_pl.po
#echo "it: "; msgfmt -o /dev/null --statistics $POFILES/klog_it.po

#lconvert -if po -of ts -i $POFILES/klog_ja.ts -o $TRANSLATIONS/klog_ja.po
for file in $POFILES/*.po
do
 echo Importing "$file"
 #echo "../po/klog_es.po" | sed 's:^\.\./po/::'
 NEWNAME=`echo "$file" |sed 's:^po/::' `
 echo $NEWNAME
 lconvert -if po -of ts -i "$file" -o "$TRANSLATIONS"/${NEWNAME%.po}.ts
done
 