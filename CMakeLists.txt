cmake_minimum_required(VERSION 3.22)
project(KLog VERSION 2.5 LANGUAGES CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_compile_definitions(KLOG_USE_VERSION_H)

# Disallow in-source builds
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed. Use: cmake -S . -B build")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build output directories for binaries and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Ensure consistency across multi-config generators (MSVC, Xcode)
foreach(cfg IN ITEMS Debug Release RelWithDebInfo MinSizeRel)
  string(TOUPPER "${cfg}" cfgU)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfgU} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${cfgU} "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${cfgU} "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
endforeach()

# Generate version.h into the build directory
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/src")
set(APP_VERSION "${PROJECT_VERSION}")
set(APP_PKGVERSION "2.5-RC3")

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in"
  "${CMAKE_BINARY_DIR}/src/version.h"
  @ONLY
)

# OS Detection and Hamlib path setup for Windows
if(WIN32)
    message(STATUS "Operating System: Windows")
elseif(APPLE)
    message(STATUS "Operating System: macOS")
elseif(UNIX)
    message(STATUS "Operating System: Linux/Unix")
endif()

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Find required Qt6 modules
find_package(Qt6 REQUIRED COMPONENTS
    Charts Core Gui Location Network Positioning
    PrintSupport Qml Quick QuickWidgets SerialPort Sql Widgets
)

qt_standard_project_setup()

# --- BUILD TARGETS ---
add_subdirectory(src)

# Option to enable/disable tests
option(BUILD_TESTING "Build the testing tree." ON)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# --- CPACK CONFIGURATION ---
set(CPACK_PACKAGE_NAME "KLog")
set(CPACK_PACKAGE_VENDOR "Jaime Robles")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "KLog - The Ham Radio Logging Program")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://www.klog.xyz")
set(CPACK_PACKAGE_CONTACT "jaime@robles.es")

# Platform-specific installers
if(WIN32)
    set(CPACK_GENERATOR "NSIS")
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
else()
    set(CPACK_GENERATOR "TGZ")
endif()

# License file configuration
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
endif()

include(CPack)


