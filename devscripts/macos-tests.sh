#!/bin/bash
# ***************************************************************************
#
#                         tests-macos.sh
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
# Build and run KLog tests on macOS.
# Must be executed from the devscripts/ directory.
#
# Requirements:
#   - Qt6 installed via the Qt online installer under $HOME/Qt
#     (tested with 6.7.x / 6.8.x / 6.10.x macos target)
#   - Hamlib installed via Homebrew:  brew install hamlib
#   - CMake available (Qt-bundled or system):
#       $HOME/Qt/Tools/CMake/CMake.app/Contents/bin/cmake
#       or /opt/homebrew/bin/cmake
#   - Ninja available:  brew install ninja  or via Qt Tools
#
# Usage:
#   cd devscripts
#   bash tests-macos.sh
#   bash tests-macos.sh --qt-version 6.8.3   # override Qt version
# --------------------------------------------------------------------------

set -euo pipefail

# ---------------------------------------------------------------------------
# Configuration – tweak these if your layout differs
# ---------------------------------------------------------------------------
QT_VERSION="${1:-}"          # first positional arg overrides auto-detect
QT_BASE="$HOME/Qt"

# Qt CMake binary (prefer Qt-bundled, fall back to system/Homebrew)
if [ -f "$QT_BASE/Tools/CMake/CMake.app/Contents/bin/cmake" ]; then
    CMAKE="$QT_BASE/Tools/CMake/CMake.app/Contents/bin/cmake"
else
    CMAKE="$(command -v cmake)"
fi

# Ninja: prefer Qt-bundled, then Homebrew, then system
if [ -f "$QT_BASE/Tools/Ninja/ninja" ]; then
    export PATH="$QT_BASE/Tools/Ninja:$PATH"
fi

# ---------------------------------------------------------------------------
# Auto-detect Qt version if not provided
# ---------------------------------------------------------------------------
if [ -z "$QT_VERSION" ]; then
    # Pick the most recent macos target found under $HOME/Qt/x.y.z/macos
    QT_VERSION=$(ls -d "$QT_BASE"/*/macos 2>/dev/null \
        | grep -Eo '[0-9]+\.[0-9]+\.[0-9]+' \
        | sort -V \
        | tail -1)
    if [ -z "$QT_VERSION" ]; then
        echo "ERROR: Could not detect a Qt6 macos installation under $QT_BASE"
        echo "       Install Qt via the Qt online installer or pass the version as first argument."
        exit 1
    fi
fi

QT_DIR="$QT_BASE/$QT_VERSION/macos"
Qt6_DIR="$QT_DIR/lib/cmake/Qt6"

echo "------------------------------------------------------------"
echo "  KLog tests – macOS"
echo "  Qt    : $QT_VERSION  ($QT_DIR)"
echo "  CMake : $CMAKE"
echo "  Hamlib: $(brew --prefix hamlib 2>/dev/null || echo 'brew prefix unknown')"
echo "------------------------------------------------------------"

# ---------------------------------------------------------------------------
# Navigate to repo root (one level up from devscripts/)
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
"$CMAKE" -S . -B "$BUILD_DIR" \
    -G "Ninja" \
    -DCMAKE_BUILD_TYPE=Debug \
    -DQt6_DIR="$Qt6_DIR" \
    -DBUILD_TESTING=ON

# ---------------------------------------------------------------------------
# Build only the test targets
# ---------------------------------------------------------------------------
echo ""
echo "==> Building tests..."
"$CMAKE" --build "$BUILD_DIR" --target build_tests -j "$(sysctl -n hw.logicalcpu)"

# ---------------------------------------------------------------------------
# Run tests via CTest
# ---------------------------------------------------------------------------
echo ""
echo "==> Running tests..."
cd "$BUILD_DIR"
ctest --output-on-failure -j "$(sysctl -n hw.logicalcpu)"

echo ""
echo "==> All tests done."
