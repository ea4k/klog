cmake_minimum_required(VERSION 3.22)

# Project details
project(KLog VERSION 2.5 LANGUAGES CXX)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_compile_definitions(KLOG_USE_VERSION_H)

# Prevent in-source builds to keep the source tree clean
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed. Use: cmake -S . -B build")
endif()

# Configuration for Qt and C++ standards
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Centralize build outputs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Ensure multi-config generators (like MSVC or Xcode) use the same output folders
foreach(cfg IN ITEMS Debug Release RelWithDebInfo MinSizeRel)
  string(TOUPPER "${cfg}" cfgU)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfgU} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${cfgU} "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${cfgU} "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
endforeach()

# Generate version.h header file
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/src")
set(APP_VERSION "${PROJECT_VERSION}")
set(APP_PKGVERSION "2.5-RC3")

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in"
  "${CMAKE_BINARY_DIR}/src/version.h"
  @ONLY
)

# OS Detection and platform-specific logic
if(WIN32)
    message(STATUS "Operating System: Windows")
    # Set absolute path for local Hamlib libraries
    get_filename_component(HAMLIB_LOCAL_PATH "${CMAKE_SOURCE_DIR}/../libs/win64/hamlib" ABSOLUTE)

    if(EXISTS "${HAMLIB_LOCAL_PATH}")
        set(HAMLIB_ROOT "${HAMLIB_LOCAL_PATH}")
        message(STATUS "Generic Windows Libs path found: ${HAMLIB_ROOT}")
    endif()
elseif(APPLE)
    message(STATUS "Operating System: macOS")
    # macOS RPATH handling for bundled libraries
    set(CMAKE_MACOSX_RPATH ON)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
elseif(UNIX)
    message(STATUS "Operating System: Linux/Unix")
else()
    message(STATUS "Operating System: Unknown")
endif()

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Required Qt6 Modules
find_package(Qt6 REQUIRED COMPONENTS
    Charts Core Gui Location Network Positioning
    PrintSupport Qml Quick QuickWidgets SerialPort Sql Widgets
)

qt_standard_project_setup()

# Add source code subdirectory
add_subdirectory(src)

# Testing configuration
option(BUILD_TESTING "Build the testing tree." ON)
if(BUILD_TESTING)
    add_compile_definitions(KLOG_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# ==============================================================================
# INSTALLATION AND DEPLOYMENT (Qt Deploy + CPack)
# ==============================================================================

include(GNUInstallDirs)

# Main application installation rules
# Ensure the target name matches your executable (e.g., 'klog')
install(TARGETS klog
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Generate platform-specific deployment scripts (Fixes DLLs on Win / Frameworks on Mac)
qt_generate_deploy_app_script(
    TARGET klog
    OUTPUT_SCRIPT deploy_script
    # Exclude unused or problematic SQL drivers (like Mimer) to avoid build errors
    EXCLUDE_PLUGINS
        qsqlmimer
        qsqlodbc
        qsqlpsql
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

# General CPack Settings
set(CPACK_PACKAGE_NAME "KLog")
set(CPACK_PACKAGE_VENDOR "EA4K")
set(CPACK_PACKAGE_VERSION "${APP_PKGVERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Ham Radio Logging software")

# Windows specific installer settings (NSIS)
if(WIN32)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "KLog")
    set(CPACK_NSIS_MENU_LINKS "bin/klog.exe" "KLog")
endif()

# macOS specific installer settings (DMG)
if(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
endif()

# Source code packaging settings (.tar.gz)
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")

# List of files to ignore when packaging the source code
set(CPACK_SOURCE_IGNORE_FILES
    "/.git/"
    "/.github/"
    "/build.*/"
    "/bin/"
    "/lib/"
    "/KLog\\.app/"
    "/.*\\\\.user$"
    "/.DS_Store"
    ".*~$"
    "/images/"       # Ignore raw design resources
    "/devscripts/"   # Ignore internal development scripts
    "/po/"           # Ignore legacy translation folders
)

# Load CPack (Must be the last instruction)
include(CPack)
