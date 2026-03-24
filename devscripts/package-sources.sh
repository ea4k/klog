#!/bin/bash
#***************************************************************************
#    begin                : mar 2026
#    copyright            : (C) 2026 by Jaime Robles
#    email                : jaime@robles.es
#***************************************************************************
#*****************************************************************************
# * This file is part of KLog.                                               *
# *                                                                           *
# *    KLog is free software: you can redistribute it and/or modify           *
# *    it under the terms of the GNU General Public License as published by   *
# *    the Free Software Foundation, either version 3 of the License, or      *
# *    (at your option) any later version.                                    *
# *                                                                           *
# *    KLog is distributed in the hope that it will be useful,                *
# *    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
# *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
# *    GNU General Public License for more details.                            *
# *                                                                            *
# *    You should have received a copy of the GNU General Public License       *
# *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.          *
# *                                                                            *
#*****************************************************************************
# package-sources.sh
# Create KLog source tarball for distribution (Debian, Mint, etc.)
# Works on Linux and macOS (bash)
#
# Usage:
#   ./devscripts/package-sources.sh
#   ./devscripts/package-sources.sh [GIT_REF]   # e.g. HEAD, v2.5, a1b2c3d
#
# Output: klog-<version>.tar.gz or klog-<version>-rc<n>.tar.gz
#         (created in the current working directory)
#
# Version is read from CMakeLists.txt:
#   project(KLog VERSION x.y ...)         → base version
#   set(APP_PKGVERSION "x.y")             → final release  → klog-x.y.tar.gz
#   set(APP_PKGVERSION "x.y-RC3")         → release candidate → klog-x.y-rc3.tar.gz
#

set -euo pipefail

GIT_REF="${1:-HEAD}"

# --- Locate repository root ---
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(git -C "$SCRIPT_DIR" rev-parse --show-toplevel 2>/dev/null || true)"
if [[ -z "$REPO_ROOT" ]]; then
    echo "ERROR: Not inside a git repository." >&2
    exit 1
fi

CMAKELISTS="$REPO_ROOT/CMakeLists.txt"
if [[ ! -f "$CMAKELISTS" ]]; then
    echo "ERROR: CMakeLists.txt not found: $CMAKELISTS" >&2
    exit 1
fi

# --- Parse VERSION from: project(KLog VERSION x.y LANGUAGES CXX) ---
VERSION=$(awk '/^project\(KLog VERSION/{print $3}' "$CMAKELISTS")
if [[ -z "$VERSION" ]]; then
    echo "ERROR: Could not find 'project(KLog VERSION ...)' in CMakeLists.txt" >&2
    exit 1
fi

# --- Parse APP_PKGVERSION from: set(APP_PKGVERSION "x.y-RCn") ---
PKGVERSION=$(grep 'APP_PKGVERSION' "$CMAKELISTS" | sed 's/.*"\(.*\)".*/\1/')
if [[ -z "$PKGVERSION" ]]; then
    echo "ERROR: Could not find 'set(APP_PKGVERSION ...)' in CMakeLists.txt" >&2
    exit 1
fi

# --- Build package name ---
# Final release: APP_PKGVERSION == VERSION        → klog-2.5
# RC release:    APP_PKGVERSION == "2.5-RC3"      → klog-2.5-rc3  (lowercase)
if [[ "$PKGVERSION" == "$VERSION" ]]; then
    PKGNAME="klog-${VERSION}"
else
    SUFFIX="${PKGVERSION#"${VERSION}"}"    # strip "2.5" prefix  → "-RC3"
    SUFFIX="${SUFFIX#-}"                   # strip leading dash   → "RC3"
    SUFFIX="$(echo "$SUFFIX" | tr '[:upper:]' '[:lower:]')"   # → "rc3"
    PKGNAME="klog-${VERSION}-${SUFFIX}"
fi

TARBALL="${PKGNAME}.tar.gz"
OUTPUT="$(pwd)/${TARBALL}"

# --- Warn if there are uncommitted changes ---
if ! git -C "$REPO_ROOT" diff --quiet 2>/dev/null || \
   ! git -C "$REPO_ROOT" diff --cached --quiet 2>/dev/null; then
    echo "WARNING: There are uncommitted changes in the repository."
    echo "         They will NOT be included in the tarball (git archive uses committed state)."
    echo ""
fi

# --- Create tarball via git archive ---
echo "Packaging KLog sources"
echo "  CMakeLists VERSION : ${VERSION}"
echo "  APP_PKGVERSION     : ${PKGVERSION}"
echo "  Package name       : ${PKGNAME}"
echo "  Git ref            : ${GIT_REF}"
echo "  Output             : ${OUTPUT}"
echo ""

git -C "$REPO_ROOT" archive \
    --format=tar.gz \
    --prefix="${PKGNAME}/" \
    "${GIT_REF}" \
    -o "$OUTPUT"

echo "Done: ${TARBALL}"
