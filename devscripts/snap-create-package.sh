#!/bin/bash
# Script to build the KLog snap package
# Can be run locally or called from Jenkins/GitHub Actions
#
# Usage:
#   cd devscripts
#   bash snap-create-package.sh
#
# Requirements:
#   - snapd, snapcraft and a build backend (LXD is the default)
#         sudo snap install snapcraft --classic
#         sudo snap install lxd && sudo lxd init --auto
#         sudo usermod -a -G lxd "$USER"     # log out and back in afterwards
#   - Must be run from the devscripts directory
#
# The recipe lives in snap/snapcraft.yaml and builds against core26
# (Ubuntu 26.04), which is the first Ubuntu release carrying Qt6 Location.
#
#***************************************************************************
#
#                         snap-create-package.sh
#                          -------------------
#    begin                : aug 2026
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

# --- Read version from CMakeLists.txt (same source the recipe uses) ---
KLOG_VERSION=$(grep 'APP_PKGVERSION' "$PROJECT_DIR/CMakeLists.txt" | sed 's/.*"\(.*\)".*/\1/')
if [ -z "$KLOG_VERSION" ]; then
    KLOG_VERSION=$(grep 'project(KLog VERSION' "$PROJECT_DIR/CMakeLists.txt" | awk '{print $3}')
fi

echo "Building KLog $KLOG_VERSION snap package"

# --- Check the toolchain is there ---
if ! command -v snapcraft >/dev/null 2>&1; then
    echo "ERROR: snapcraft not found. Install it with:"
    echo "         sudo snap install snapcraft --classic"
    exit 1
fi

ARCH=$(dpkg --print-architecture 2>/dev/null || uname -m)
echo "Architecture: $ARCH"

# --- Clean previous build ---
echo "[1/3] Cleaning..."
rm -f "$DEVSCRIPTS_DIR"/klog_*.snap
(cd "$PROJECT_DIR" && snapcraft clean)

# --- Build ---
echo "[2/3] Building the snap (this pulls the core26 and mesa-2604 snaps the"
echo "      first time and takes a while)..."
(cd "$PROJECT_DIR" && snapcraft pack)

# --- Find whatever snapcraft generated and move it next to the other packages ---
echo "[3/3] Collecting the artifact..."
SNAP_FILE=$(find "$PROJECT_DIR" -maxdepth 1 -name "klog_*.snap" | head -1)
if [ -z "$SNAP_FILE" ]; then
    echo "ERROR: snapcraft did not generate any .snap file"
    exit 1
fi

# --- Check the translations made it into the package ---
# The .qm files are installed into usr/share/klog/translations, which is where
# KLog looks for them. If LinguistTools is missing they are silently skipped,
# and KLog would be shipped in English only.
if command -v unsquashfs >/dev/null 2>&1; then
    if ! unsquashfs -l "$SNAP_FILE" | grep -q "usr/share/klog/translations/klog_.*\.qm"; then
        echo "ERROR: $SNAP_FILE contains no KLog translation files"
        echo "       Check that Qt6 LinguistTools (lrelease) is available."
        exit 1
    fi
else
    echo "WARNING: unsquashfs not available, skipping the translations check"
fi

mv "$SNAP_FILE" "$DEVSCRIPTS_DIR/"
FINAL_NAME=$(basename "$SNAP_FILE")

echo ""
echo "Done! KLog $KLOG_VERSION -> devscripts/$FINAL_NAME"
echo ""
echo "Install it locally with:"
echo "    sudo snap install --dangerous devscripts/$FINAL_NAME"
echo ""
echo "Then connect the interfaces that are not connected automatically:"
echo "    sudo snap connect klog:raw-usb"
echo "    sudo snap connect klog:cups-control"
echo "    snap interface serial-port          # find your radio's slot"
echo "    sudo snap connect klog:serial-port <slot>"
