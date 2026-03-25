@echo off
rem Script to deploy the windows package file
rem Must be executed from the devscripts directory of KLog
rem ***************************************************************************
rem
rem                         win64-create-package.bat
rem                          -------------------
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
rem *****************************************************************************

SETLOCAL EnableDelayedExpansion

rem --- Environment setup ---
echo Setting up environment for Qt usage...
set PATH=%PATH%;C:\Qt\6.8.3\mingw_64\bin;C:\Qt\Tools\mingw1310_64\bin
set PATH=%PATH%;C:\Program Files\InstallBuilder Enterprise 23.10.1\bin
set PATH=%PATH%;C:\Qt\Tools\CMake_64\bin;C:\Qt\Tools\Ninja

rem --- Dependency paths ---
set HAMLIB_DIR=C:\hamlib-w64-4.7.0
set OPENSSL_DIR=C:\Qt\Tools\OpenSSLv3\Win_x64

rem --- Read PKGVERSION from CMakeLists.txt ---
rem :: Splits line  set(APP_PKGVERSION "x.y")  by " and takes token 2
set KLOGDEVELVERSION=unknown
for /f "tokens=2 delims=^"" %%v in ('findstr /C:"APP_PKGVERSION" ..\CMakeLists.txt') do set KLOGDEVELVERSION=%%v
echo Building KLog %KLOGDEVELVERSION%

rem --- Clean previous installer from devscripts ---
del klog-*win64*.exe 2>nul

rem --- Go to project root ---
cd ..

rem --- Clean previous build artifacts ---
rmdir /S /Q build 2>nul
rmdir /S /Q src\release 2>nul

rem --- [1/4] CMake configure ---
echo [1/4] Configuring with CMake...
cmake -S . -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF
if %errorlevel% neq 0 (
    echo ERROR: CMake configuration failed
    exit /b 1
)

rem --- [2/4] CMake build ---
echo [2/4] Building...
cmake --build build -j 4
if %errorlevel% neq 0 (
    echo ERROR: Build failed
    exit /b 1
)

rem --- [3/4] Prepare release directory ---
echo [3/4] Preparing release directory...
mkdir src\release
copy /Y build\bin\klog.exe src\release\

rem --- Copy OpenSSL and Hamlib DLLs ---
rem :: OpenSSL DLLs must match the version used to build Qt.
rem :: Check main.cpp for the SSL version details.
copy /Y "%OPENSSL_DIR%\bin\*.dll" src\release\
copy /Y "%HAMLIB_DIR%\bin\*.dll" src\release\

rem --- Deploy Qt runtime ---
windeployqt6 --dir src\release --release --compiler-runtime --qmldir src\qml src\release\klog.exe
if %errorlevel% neq 0 (
    echo ERROR: windeployqt6 failed
    exit /b 1
)

rem --- [4/4] Build installer ---
echo [4/4] Building installer package...
cd devscripts
builder-cli.exe build build-win64.xml --verbose

echo.
echo Done^^! KLog %KLOGDEVELVERSION% installer ready.
