#!/bin/bash
# ***************************************************************************
#
#                         tests-linux.sh
#                         -------------------
#    begin                : mar 2026
#    copyright            : (C) 2026 by Jaime Robles
#    email                : jaime@robles.es
# ***************************************************************************
# *****************************************************************************
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
# *****************************************************************************
#
# Build and run KLog tests on Linux.
# Must be executed from the devscripts/ directory.
#
# Requirements (Debian/Ubuntu example):
#   sudo apt install cmake ninja-build libhamlib-dev \
#        qt6-base-dev qt6-tools-dev libqt6serialport6-dev \
#        libqt6charts6-dev qt6-location-dev
#
# For a Qt6 installed via the Qt online installer under $HOME/Qt, the script
# will prefer that installation over the system one.
#
# Usage:
#   cd devscripts
#   bash tests-linux.sh
#   bash tests-linux.sh --qt-version 6.8.3   # override Qt version
# --------------------------------------------------------------------------

set -euo pipefail

# ---------------------------------------------------------------------------
# Configuration
# ---------------------------------------------------------------------------
QT_VERSION="${1:-}"
QT_BASE="$HOME/Qt"

# ---------------------------------------------------------------------------
# Auto-detect Qt version from the Qt online installer layout, or fall back to
# the system cmake/qt6 toolchain (no Qt6_DIR override needed in that case).
# ---------------------------------------------------------------------------
USE_SYSTEM_QT=false

if [ -z "$QT_VERSION" ]; then
    QT_VERSION=$(ls -d "$QT_BASE"/*/gcc_64 2>/dev/null \
        | grep -Eo '[0-9]+\.[0-9]+\.[0-9]+' \
        | sort -V \
        | tail -1)
    if [ -z "$QT_VERSION" ]; then
        echo "INFO: No Qt online-installer layout found under $QT_BASE/x.y.z/gcc_64"
        echo "      Falling back to system Qt6 (pkg-config / cmake system paths)."
        USE_SYSTEM_QT=true
    fi
fi

if [ "$USE_SYSTEM_QT" = false ]; then
    QT_DIR="$QT_BASE/$QT_VERSION/gcc_64"
    Qt6_DIR="$QT_DIR/lib/cmake/Qt6"
    # Put Qt tools on PATH so cmake can find moc, rcc, uic, etc.
    export PATH="$QT_DIR/bin:$PATH"
fi

# Prefer cmake from the Qt installer if present, otherwise use system cmake
if [ -f "$QT_BASE/Tools/CMake/bin/cmake" ]; then
    CMAKE="$QT_BASE/Tools/CMake/bin/cmake"
else
    CMAKE="$(command -v cmake)"
fi

# Prefer Ninja from the Qt installer
if [ -f "$QT_BASE/Tools/Ninja/ninja" ]; then
    export PATH="$QT_BASE/Tools/Ninja:$PATH"
fi

echo "------------------------------------------------------------"
echo "  KLog tests – Linux"
if [ "$USE_SYSTEM_QT" = true ]; then
    echo "  Qt    : system"
else
    echo "  Qt    : $QT_VERSION  ($QT_DIR)"
fi
echo "  CMake : $CMAKE"
echo "  Hamlib: $(pkg-config --modversion hamlib 2>/dev/null || echo 'pkg-config not available')"
echo "------------------------------------------------------------"

# ---------------------------------------------------------------------------
# Navigate to repo root
# ---------------------------------------------------------------------------
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
BUILD_DIR="$REPO_ROOT/build-tests"

cd "$REPO_ROOT"

# ---------------------------------------------------------------------------
# Configure
# ---------------------------------------------------------------------------
echo ""
echo "==> Configuring..."

CMAKE_EXTRA_ARGS=()
if [ "$USE_SYSTEM_QT" = false ]; then
    CMAKE_EXTRA_ARGS+=("-DQt6_DIR=$Qt6_DIR")
fi

"$CMAKE" -S . -B "$BUILD_DIR" \
    -G "Ninja" \
    -DCMAKE_BUILD_TYPE=Debug \
    -DBUILD_TESTING=ON \
    "${CMAKE_EXTRA_ARGS[@]}"

# ---------------------------------------------------------------------------
# Build only the test targets
# ---------------------------------------------------------------------------
echo ""
echo "==> Building tests..."
"$CMAKE" --build "$BUILD_DIR" --target build_tests -j "$(nproc)"

# ---------------------------------------------------------------------------
# Run tests via CTest
# ---------------------------------------------------------------------------
echo ""
echo "==> Running tests..."
cd "$BUILD_DIR"
ctest --output-on-failure -j "$(nproc)"

echo ""
echo "==> All tests done."
