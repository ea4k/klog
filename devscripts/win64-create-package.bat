echo off
rem Script to deploy the windows package file
rem Must be executed in the build directory of KLog
rem ***************************************************************************
rem			
rem                         klogwincreatepacksage2.bat
rem						  -------------------
rem    begin                : jan 2021
rem    copyright            : (C) 2021 by Jaime Robles
rem    email                : jaime@robles.es
rem ***************************************************************************
rem /*****************************************************************************
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
set KLOGDEVELVERSION=1
set PATH=%PATH%;C:\Qt\5.15.2\mingw81_64\bin;C:\Qt\Tools\mingw810_64\bin;
set PATH=%PATH%;C:\Program Files\InstallBuilder Enterprise 23.10.1\bin;
echo Linea 10
cd ../src/
rmdir /S /Q build
rmdir /S /Q release
rmdir /S /Q debug
echo Linea 20
mingw32-make clean
echo Linea 21
for /f "tokens=2 delims=\=" %%a in ('type src.pro^|find "PKGVERSION = "') do (
  set KLOGDEVELVERSION=%%a & goto :continue
)
echo Linea 30
:continue
@echo on
SETLOCAL EnableDelayedExpansion
set KLOGDEVELVERSION=!KLOGDEVELVERSION: =!
rem set KLOGDEVELVERSION=%KLOGDEVELVERSION:~1%
echo Building KLog-%KLOGDEVELVERSION%
echo Linea 40
qmake -set CONFIG+=x86_64
qmake src.pro
echo Linea 41
mingw32-make
echo Linea 42
mkdir release
echo Linea 43
xcopy /Y /S /F build\target\* release
echo localdir=%cd%
echo %localdir%
copy ..\..\paquete\openssl\openssl-3\x64\bin\*.dll release
copy ..\..\libs\hamlib-w64\bin\*.dll release
windeployqt --qmldir qml release\klog.exe
:: The SSL DLLs must be included and must match the version that were used to build Qt.
:: Check in main.cpp and uncomment the SSL line to see what is the version that was used.
:: After knowing the version, the package can be obtained from: https://indy.fulgan.com/SSL/Archive/

rem cd ..\..
echo %cd%
cd ../devscripts
echo %cd%
builder-cli.exe build build-win64.xml --verbose



