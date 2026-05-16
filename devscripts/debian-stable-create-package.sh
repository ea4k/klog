#!/bin/bash
# Script to build the Debian stable .deb package using CPack
# Can be run locally or called from Jenkins
#
# Usage:
#   cd devscripts
#   bash debian-stable-create-package.sh
#
# Requirements:
#   - Qt6 development packages installed (apt install qt6-base-dev ...)
#   - cmake, ninja-build installed
#   - Must be run from the devscripts directory
#
#***************************************************************************
#
#                         debian-stable-create-package.sh
#                          -------------------
#    begin                : may 2026
#    copyright            : (C) 2026 by Jaime Robles
#    email                : jaime@robles.es
#
#*****************************************************************************
# * This file is part of KLog.                                               *
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
#*****************************************************************************/

set -e

DEVSCRIPTS_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$(dirname "$DEVSCRIPTS_DIR")"

# --- Read version from CMakeLists.txt ---
KLOG_VERSION=$(grep 'APP_PKGVERSION' "$PROJECT_DIR/CMakeLists.txt" | sed 's/.*"\(.*\)".*/\1/')
if [ -z "$KLOG_VERSION" ]; then
    KLOG_VERSION=$(grep 'project(KLog VERSION' "$PROJECT_DIR/CMakeLists.txt" | awk '{print $3}')
fi

echo "Building KLog $KLOG_VERSION .deb package for Debian stable"

# --- Detect architecture ---
ARCH=$(dpkg --print-architecture)
echo "Architecture: $ARCH"

# --- Expected output filename ---
DEB_NAME="klog_${KLOG_VERSION}_${ARCH}.deb"

# --- Clean previous build ---
echo "[1/4] Cleaning..."
rm -rf "$PROJECT_DIR/build"
rm -f "$DEVSCRIPTS_DIR"/klog_*.deb
rm -f "$DEVSCRIPTS_DIR"/klog_*.tar.gz

# --- CMake configure ---
echo "[2/4] Configuring with CMake..."
cmake -S "$PROJECT_DIR" -B "$PROJECT_DIR/build" \
    -G "Ninja" \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_TESTING=OFF

# --- Build ---
echo "[3/4] Building..."
cmake --build "$PROJECT_DIR/build" -j "$(nproc)"

# --- Generate .deb with CPack ---
echo "[4/4] Generating .deb package with CPack..."
cd "$PROJECT_DIR/build"
cpack -G DEB

# --- Move .deb to devscripts directory ---
mv "$PROJECT_DIR/build/"*.deb "$DEVSCRIPTS_DIR/$DEB_NAME"

echo ""
echo "Done! KLog $KLOG_VERSION -> devscripts/$DEB_NAME"
