echo off
rem Script to deploy the windows package file
rem Must be executed in the build directory of KLog
rem***************************************************************************
rem			
rem                         klogwincreatepacksage2.bat
rem						  -------------------
rem    begin                : jan 2021
rem    copyright            : (C) 2021 by Jaime Robles
rem    email                : jaime@robles.es
rem ***************************************************************************
rem/*****************************************************************************
rem * This file is part of KLog.                                                *
rem *                                                                           *
rem *    KLog is free software: you can redistribute it and/or modify           *
rem *    it under the terms of the GNU General Public License as published by   *
rem *    the Free Software Foundation, either version 3 of the License, or      *
rem *    (at your option) any later version.                                    *
rem *                                                                           *
rem *    KLog is distributed in the hope that it will be useful,                *
rem *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
rem *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
rem *    GNU General Public License for more details.                           *
rem *                                                                           *
rem *    You should have received a copy of the GNU General Public License      *
rem *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
rem *                                                                           *
rem *****************************************************************************/@echo off
echo Setting up environment for Qt usage...
set PATH=C:/Qt/5.15.2/mingw81_32/bin;C:/Qt/Tools/mingw810_32/bin;%PATH%
cd ../src/
rmdir /S /Q build
rmdir /S /Q release
rmdir /S /Q debug
mingw32-make clean

qmake src.pro
mingw32-make
mkdir release
xcopy /Y /S /F build\target release
echo localdir=%cd%
echo %localdir%
copy ..\..\paquete\openssl\*.dll release
copy ..\..\paquete\hamlibDLL\*.dll release
windeployqt --qmldir qml release\klog.exe
:: The SSL DLLs must be included and must match the version that were used to build Qt.
:: Check in main.cpp and uncomment the SSL line to see what is the version that was used.
:: After knowing the version, the package can be obtained from: https://indy.fulgan.com/SSL/Archive/

copy *.ico release
copy AUTHORS release
copy Changelog release
copy NEWS release
copy TODO release
copy README release
copy COPYING release
copy gpl-3.0-standalone.html release
rem cd ..\..



