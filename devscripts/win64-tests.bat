@echo off
rem ***************************************************************************
rem
rem                          tests-win.bat
rem                          -------------------
rem     begin                : mar 2026
rem     copyright            : (C) 2026 by Jaime Robles
rem     email                : jaime@robles.es
rem ***************************************************************************
rem  *****************************************************************************
rem  * This file is part of KLog.                                                *
rem  *                                                                           *
rem  *    KLog is free software: you can redistribute it and/or modify           *
rem  *    it under the terms of the GNU General Public License as published by   *
rem  *    the Free Software Foundation, either version 3 of the License, or      *
rem  *    (at your option) any later version.                                    *
rem  *                                                                           *
rem  *    KLog is distributed in the hope that it will be useful,                *
rem  *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
rem  *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
rem  *    GNU General Public License for more details.                           *
rem  *                                                                           *
rem  *    You should have received a copy of the GNU General Public License      *
rem  *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
rem  *                                                                           *
rem  *****************************************************************************
rem
rem  Build and run KLog tests on Windows (64-bit, MinGW).
rem  Must be executed from the devscripts\ directory.
rem
rem  Requirements:
rem    - Qt 6 installed via the Qt online installer under C:\Qt
rem      (MinGW 64-bit target, e.g. C:\Qt\6.8.3\mingw_64)
rem    - MinGW toolchain bundled with Qt (C:\Qt\Tools\mingw1310_64)
rem    - Hamlib for Windows placed at:
rem        ..\libs\win64\hamlib\   (relative to the klog repo root)
rem      OR set the environment variable HAMLIB_ROOT before running this script.
rem      Hamlib Windows releases: https://github.com/Hamlib/Hamlib/releases
rem
rem  Usage (from a plain cmd.exe, no need for a VS developer prompt):
rem    cd devscripts
rem    tests-win.bat
rem    tests-win.bat 6.8.3        REM override Qt version
rem --------------------------------------------------------------------------

setlocal ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION

rem ---------------------------------------------------------------------------
rem  Qt version – first argument or autodetect
rem ---------------------------------------------------------------------------
set "QT_VERSION=%~1"
set "QT_BASE=C:\Qt"

if "%QT_VERSION%"=="" (
    rem Try to find the most recent mingw_64 installation
    set "QT_VERSION="
    for /d %%D in ("%QT_BASE%\*") do (
        if exist "%%D\mingw_64\bin\qmake.exe" (
            set "QT_VERSION=%%~nxD"
        )
    )
    if "!QT_VERSION!"=="" (
        echo ERROR: Could not detect a Qt6 MinGW installation under %QT_BASE%
        echo        Install Qt via the Qt online installer or pass the version as first argument.
        exit /b 1
    )
)

set "QT_DIR=%QT_BASE%\%QT_VERSION%\mingw_64"
set "QT6_CMAKE_DIR=%QT_DIR%\lib\cmake\Qt6"

rem ---------------------------------------------------------------------------
rem  Toolchain paths
rem ---------------------------------------------------------------------------
rem MinGW bundled with Qt (adjust the name if your Qt ships a different version)
set "MINGW_DIR=%QT_BASE%\Tools\mingw1310_64\bin"

rem CMake and Ninja from Qt installer
set "CMAKE_DIR=%QT_BASE%\Tools\CMake_64\bin"
set "NINJA_DIR=%QT_BASE%\Tools\Ninja"

set "PATH=%QT_DIR%\bin;%MINGW_DIR%;%CMAKE_DIR%;%NINJA_DIR%;%PATH%"

rem ---------------------------------------------------------------------------
rem  Hamlib location
rem  Priority: env var HAMLIB_ROOT > sibling libs\win64\hamlib > Program Files
rem ---------------------------------------------------------------------------
if not "%HAMLIB_ROOT%"=="" (
    set "HAMLIB_DIR=%HAMLIB_ROOT%"
    goto hamlib_found
)

rem Sibling layout expected by CMakeLists.txt:  <repo>/../libs/win64/hamlib
set "SCRIPT_DIR=%~dp0"
set "REPO_ROOT=%SCRIPT_DIR%.."
for %%I in ("%REPO_ROOT%") do set "REPO_ROOT=%%~fI"
set "HAMLIB_SIBLING=%REPO_ROOT%\..\libs\win64\hamlib"
for %%I in ("%HAMLIB_SIBLING%") do set "HAMLIB_SIBLING=%%~fI"
if exist "%HAMLIB_SIBLING%\include\hamlib\rig.h" (
    set "HAMLIB_DIR=%HAMLIB_SIBLING%"
    goto hamlib_found
)

rem Fall back: first hamlib-w64-* folder found under Program Files
for /d %%D in ("C:\Program Files\hamlib-w64-*") do (
    if exist "%%D\include\hamlib\rig.h" (
        set "HAMLIB_DIR=%%D"
        goto hamlib_found
    )
)

echo ERROR: Hamlib not found.
echo        Place Hamlib at  %HAMLIB_SIBLING%
echo        or set the HAMLIB_ROOT environment variable.
echo        Download from: https://github.com/Hamlib/Hamlib/releases
exit /b 1

:hamlib_found

rem ---------------------------------------------------------------------------
rem  Summary
rem ---------------------------------------------------------------------------
echo ------------------------------------------------------------
echo   KLog tests - Windows
echo   Qt      : %QT_VERSION%  (%QT_DIR%)
echo   MinGW   : %MINGW_DIR%
echo   CMake   : %CMAKE_DIR%
echo   Hamlib  : %HAMLIB_DIR%
echo ------------------------------------------------------------

rem ---------------------------------------------------------------------------
rem  Navigate to the repo root (one level above devscripts\)
rem ---------------------------------------------------------------------------
set "BUILD_DIR=%REPO_ROOT%\build-tests"
cd /d "%REPO_ROOT%"

rem ---------------------------------------------------------------------------
rem  Configure
rem ---------------------------------------------------------------------------
echo.
echo =^> Configuring...
cmake -S . -B "%BUILD_DIR%" ^
    -G "Ninja" ^
    -DCMAKE_BUILD_TYPE=Debug ^
    -DQt6_DIR="%QT6_CMAKE_DIR%" ^
    -DHAMLIB_ROOT="%HAMLIB_DIR%" ^
    -DBUILD_TESTING=ON
if errorlevel 1 (
    echo ERROR: CMake configuration failed.
    exit /b 1
)

rem ---------------------------------------------------------------------------
rem  Build test targets
rem ---------------------------------------------------------------------------
echo.
echo =^> Building tests...
cmake --build "%BUILD_DIR%" --target build_tests -j %NUMBER_OF_PROCESSORS%
if errorlevel 1 (
    echo ERROR: Build failed.
    exit /b 1
)

rem ---------------------------------------------------------------------------
rem  Run tests via CTest
rem ---------------------------------------------------------------------------
echo.
echo =^> Running tests...
cd /d "%BUILD_DIR%"
ctest --output-on-failure -j %NUMBER_OF_PROCESSORS%
if errorlevel 1 (
    echo ERROR: One or more tests failed.
    exit /b 1
)

echo.
echo =^> All tests done.
endlocal
