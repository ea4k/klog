#!/bin/bash
# Script to deploy package the sources release tar.gz file
# Must be executed in the build directory of KLog
#***************************************************************************
#			
#                         Jenkins-klog-package-sources.sh
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
KLOG_VERSION=`grep "VERSION =" src/src.pro |awk '{print $3}'`
echo "Packaging KLog-$KLOG_VERSION"
export CXXFLAGS=-std=c++11
rm src/README.md

NEW_NAME=klog-$KLOG_VERSION
mv src "$NEW_NAME"
if [ $? -ne 0 ]; then
echo "There was an error when copying the KLog folder"
exit 1
fi

tar cvzf "$NEW_NAME".tar.gz "$NEW_NAME"
if [ $? -eq 0 ]; then
echo "You can find the tar.gz file in this folder... enjoy KLog!"
exit 0
else
çecho "There was an error when packaging KLog"
exit 1
fi
