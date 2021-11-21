#!/bin/sh
# Script to convert ts files into po
# Must be executed in the build directory of KLog
#***************************************************************************
#			
#                         translations-ts2po.sh
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
# *****************************************************************************/SOURCES="src"
SOURCES="src"
TRANSLATIONS="src/translations"
POFILES="po"
export QT_SELECT="qt5"
lupdate -no-obsolete $SOURCES/src.pro


for file in $TRANSLATIONS/*.ts
 do
  echo Exporting "$file"
  NEWNAME=echo $($file |sed 's:^../ts/::')
  #NEWNAME=`echo "$file" |sed 's:^../ts/::'`
  echo "$NEWNAME"
  lconvert -if ts -of po -i "$file" -o ${NEWNAME%.ts}.po
  mv ${NEWNAME%.ts}.po "$POFILES"
 done

mv $POFILES/klog_en.po $POFILES/klog.pot

# Removing the actively TS maintained files:
#rm $POFILES/klog_es.po
#rm $POFILES/klog_ja.po

echo; echo;
echo "Statistics of the po files"
echo

for file2 in "$POFILES"/*.po
 do
  echo Analizing "$file2"
  msgfmt -o /dev/null --statistics "$file2"
done
                                                  



#podebconf-report-po -v --notdebconf --package=klog --potfile=klog.pot --podir=$POFILES --nodeadline --call=debian-i18n@lists.debian.org --noforce --deadline="+10 days"

