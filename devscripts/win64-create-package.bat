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

rem --- Force MinGW compiler, ignore any LLVM/Clang in PATH ---
set CC=C:\Qt\Tools\mingw1310_64\bin\gcc.exe
set CXX=C:\Qt\Tools\mingw1310_64\bin\g++.exe

rem --- Dependency paths ---
set HAMLIB_DIR=C:\hamlib-w64-4.7.0
set OPENSSL_DIR=C:\Qt\Tools\OpenSSLv3\Win_x64

rem --- Read PKGVERSION from CMakeLists.txt ---
set KLOGDEVELVERSION=unknown
rem for /f "delims=" %%v in ('powershell -NoProfile -Command "([regex]::Match((gc ..\CMakeLists.txt -Raw),'APP_PKGVERSION\s+""([^""]+)""')).Groups[1].Value"') do set KLOGDEVELVERSION=%%v
rem for /f "tokens=2 delims=^"" %%v in ('findstr /C:"APP_PKGVERSION" ..\CMakeLists.txt') do set KLOGDEVELVERSION=%%v
for /f "tokens=3 delims=( " %%v in ('type ..\CMakeLists.txt ^| find "APP_PKGVERSION"') do set _KLOGVER=%%v
if defined _KLOGVER set KLOGDEVELVERSION=%_KLOGVER:~1,-2%

echo Building KLog %KLOGDEVELVERSION%

rem --- Expose the version to the Jenkinsfile so it can archive/upload the ---
rem --- exact installer filename instead of a broad "*win64*.exe" glob.    ---
echo %KLOGDEVELVERSION%> version.txt

rem --- Clean previous installers from devscripts (any leftover version) ---
del /Q KLog-*-win64.exe 2>nul

rem --- Go to project root ---
cd ..

rem --- Clean previous build artifacts ---
rmdir /S /Q build 2>nul
rmdir /S /Q src\release 2>nul

rem --- [1/4] CMake configure ---
echo [1/4] Configuring with CMake...
cmake -S . -B build -G "Ninja" ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DBUILD_TESTING=OFF ^
    -DCMAKE_C_COMPILER=C:/Qt/Tools/mingw1310_64/bin/gcc.exe ^
    -DCMAKE_CXX_COMPILER=C:/Qt/Tools/mingw1310_64/bin/g++.exe

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
rem :: --no-translations: KLog never installs a Qt base translator (see
rem :: loadTranslations() in main.cpp, it only loads klog_*.qm), so the qt_*.qm
rem :: files windeployqt6 would deploy are unused and must not be packaged.
windeployqt6 --dir src\release --release --compiler-runtime --no-translations --qmldir src\qml src\release\klog.exe
if %errorlevel% neq 0 (
    echo ERROR: windeployqt6 failed
    exit /b 1
)

rem --- Deploy the KLog translations ---
rem :: qt_add_translations compiles the .ts files into build\src\klog_*.qm.
rem :: windeployqt6 does not deploy them, so the KLog ones have to be copied by
rem :: hand into the translations folder next to klog.exe, which is where KLog
rem :: looks for them (Utilities::getTranslationSearchPaths).
echo Deploying KLog translations...
if not exist src\release\translations mkdir src\release\translations
copy /Y build\src\klog_*.qm src\release\translations\
if %errorlevel% neq 0 (
    echo ERROR: No KLog translation files ^(klog_*.qm^) found in build\src
    exit /b 1
)

rem --- [4/4] Build installer ---
echo [4/4] Building installer package...
cd devscripts
builder-cli.exe build build-win64.xml --verbose

echo.
echo Done^^! KLog %KLOGDEVELVERSION% installer ready.
