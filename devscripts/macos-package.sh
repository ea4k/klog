#!/bin/bash

# Stop the script immediately if any command fails
set -e

echo "🍏 Starting KLog packaging for macOS..."

# 1. Resolve the project root path dynamically
# This allows the script to be executed from any directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT"

# 2. Configuration
# Ensure these paths match your environment 
TRANS_DIR="src/translations"
QT_PATH="${HOME}/Qt/6.10.2/macos"
BUILD_DIR="build_macos"

echo "🧰 Using Qt at: $QT_PATH"

# ==============================================================================
# 3. TRANSLATIONS UPDATE (lupdate & lrelease)
# ==============================================================================
echo "🌐 Updating and compiling translations..."

# Check if the translation directory exists
if [ ! -d "$TRANS_DIR" ]; then
    echo "❌ Error: Translation directory '$TRANS_DIR' not found."
    exit 1
fi

# 3.1 Run lupdate to extract new strings from src/
# -no-obsolete: removes strings that are no longer in the source code
"$QT_PATH/bin/lupdate" src/ -no-obsolete -ts "$TRANS_DIR"/*.ts

# 3.2 Run lrelease to compile .ts files into binary .qm files
"$QT_PATH/bin/lrelease" "$TRANS_DIR"/*.ts

echo "✅ Translations updated and compiled successfully!"
# ==============================================================================

# 4. Clean previous builds to ensure a 100% clean .dmg
if [ -d "$BUILD_DIR" ]; then
    echo "🧹 Cleaning previous build directory ($BUILD_DIR)..."
    rm -rf "$BUILD_DIR"
fi

# 5. Configure the project with CMake (Forcing Release mode) 
echo "⚙️ Configuring the project..."
cmake -S . -B "$BUILD_DIR" \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_PREFIX_PATH="$QT_PATH" \
      -DBUILD_TESTING=OFF

# 6. Build using all available CPU cores 
CORES=$(sysctl -n hw.ncpu)
echo "🔨 Building KLog using $CORES cores..."
cmake --build "$BUILD_DIR" --config Release -j"$CORES"

# 7. Package the .dmg using CPack [cite: 4, 7]
echo "📦 Packaging the application into a .dmg file..."
cd "$BUILD_DIR"

# We use CPack to trigger the DragNDrop generator defined in CMakeLists.txt [cite: 7]
cpack -G DragNDrop -C Release

echo ""
echo "✅ Packaging completed successfully!"
echo "🚀 Your .dmg file is ready in: $PROJECT_ROOT/$BUILD_DIR/"
