#!/bin/bash
# Script to deploy the macOS package
# Must be executed from the devscripts directory of KLog
#***************************************************************************
#
#                         macOSDeploy-cmake.sh
#                          -------------------
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

set -e

DEVSCRIPTS_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$(dirname "$DEVSCRIPTS_DIR")"

# --- Read version: APP_PKGVERSION takes priority over PROJECT VERSION ---
KLOG_VERSION=$(grep 'APP_PKGVERSION' "$PROJECT_DIR/CMakeLists.txt" | sed 's/.*"\(.*\)".*/\1/')
if [ -z "$KLOG_VERSION" ]; then
    KLOG_VERSION=$(grep 'project(KLog VERSION' "$PROJECT_DIR/CMakeLists.txt" | awk '{print $3}')
fi
echo "Packaging KLog $KLOG_VERSION"

# --- Qt environment ---
QT_VERSION="6.10.2"
QT_DIR="$HOME/Qt/$QT_VERSION/macos"
export PATH="$HOME/Qt/Tools/Ninja:$QT_DIR/bin:$PATH"
CMAKE_BIN="$HOME/Qt/Tools/CMake/CMake.app/Contents/bin/cmake"

# --- Bundle name must match OUTPUT_NAME set in src/CMakeLists.txt ---
APP_NAME="KLog"
APP="$PROJECT_DIR/build/bin/${APP_NAME}.app"

# --- Clean previous build ---
echo "[1/4] Cleaning..."
rm -rf "$PROJECT_DIR/build"

# --- CMake configure ---
echo "[2/4] Configuring with CMake..."
"$CMAKE_BIN" -S "$PROJECT_DIR" -B "$PROJECT_DIR/build" \
    -G "Ninja" \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_TESTING=OFF \
    -DCMAKE_PREFIX_PATH="$QT_DIR"

# --- Build ---
echo "[3/4] Building..."
"$CMAKE_BIN" --build "$PROJECT_DIR/build" -j 4

# --- Check the translations made it into the bundle ---
# CMake copies build/src/klog_*.qm into Contents/Resources/translations, which is
# where KLog looks for them. If LinguistTools is missing they are silently
# skipped, and KLog would be shipped in English only.
if ! ls "$APP/Contents/Resources/translations"/klog_*.qm >/dev/null 2>&1; then
    echo "ERROR: No KLog translations in $APP/Contents/Resources/translations"
    echo "       Check that Qt6 LinguistTools (lrelease) is available."
    exit 1
fi

# --- Deploy Qt into the bundle and create DMG ---
echo "[4/4] Deploying Qt and creating DMG..."

# Run from build/bin and pass a relative app name: macdeployqt derives the
# DMG volume name (shown as the Finder window title when the DMG is opened)
# from the path it's invoked with, so an absolute path leaks the build
# machine's directory (e.g. the Jenkins workspace) into that title.
# https://bugreports.qt.io/browse/QTBUG-60324
(
    cd "$PROJECT_DIR/build/bin"
    "$QT_DIR/bin/macdeployqt6" "${APP_NAME}.app" \
        -qmldir="$PROJECT_DIR/src/qml" \
        -codesign="-" \
        -dmg
)

# macdeployqt6 names the DMG after the .app: KLog.dmg
mv "$PROJECT_DIR/build/bin/${APP_NAME}.dmg" "$DEVSCRIPTS_DIR/KLog-$KLOG_VERSION-arm64.dmg"

echo ""
echo "Done! KLog $KLOG_VERSION -> devscripts/KLog-$KLOG_VERSION-arm64.dmg"
