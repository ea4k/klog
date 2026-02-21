cmake_minimum_required(VERSION 3.30)
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
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Added from MacOS


# Put all build outputs under build/bin and build/lib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
# Multi-config generators (MSVC, Xcode)
foreach(cfg IN ITEMS Debug Release RelWithDebInfo MinSizeRel)
  string(TOUPPER "${cfg}" cfgU)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfgU} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${cfgU} "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${cfgU} "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
endforeach()

# Generate version.h into build/src (not source)
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/src")
set(APP_VERSION "${PROJECT_VERSION}")
set(APP_PKGVERSION "2.5-RC1")
#set(APP_PKGVERSION "2.4.2-RC3")

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in"
  "${CMAKE_BINARY_DIR}/src/version.h"
  @ONLY
)

# Detect OS and print to console
if(WIN32)
    message(STATUS "Operating System: Windows")
    # Get the path to klog/../libs/win64/hamlib and convert it to an absolute path
    get_filename_component(HAMLIB_LOCAL_PATH "${CMAKE_SOURCE_DIR}/../libs/win64/hamlib" ABSOLUTE)

    # Check if that folder actually exists before setting it
    if(EXISTS "${HAMLIB_LOCAL_PATH}")
        set(HAMLIB_ROOT "${HAMLIB_LOCAL_PATH}")
        message(STATUS "Generic Windows Libs path found: ${HAMLIB_ROOT}")
    endif()
elseif(APPLE)
    message(STATUS "Operating System: macOS")
elseif(UNIX)
    message(STATUS "Operating System: Linux/Unix")
else()
    message(STATUS "Operating System: Unknown")
endif()

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Find Qt6 modules required (based on src.pro typical dependencies)
find_package(Qt6 REQUIRED COMPONENTS
    Charts
    Core
    Gui
 #   Location
    Network
    Positioning
    PrintSupport
    Qml
    Quick
    QuickWidgets
    SerialPort
    Sql
    Widgets
    #REQUIRED
)

qt_standard_project_setup() # Added from MacOS


add_subdirectory(src)

# Define an option to build tests (Default = OFF)
option(BUILD_TESTING "Build the testing tree." ON)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

