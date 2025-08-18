cmake_minimum_required(VERSION 3.16)
project(KLog VERSION 2.5 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)


# Generate version.h from version.h.in into this (src) binary directory
#configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in
#               ${CMAKE_CURRENT_BINARY_DIR}/build/src/version.h
#               @ONLY)
set(APP_VERSION "${PROJECT_VERSION}")
set(APP_PKGVERSION "2.5-RC1")
#set(APP_PKGVERSION "2.4.2-RC3")
# Generate version.h into the build/src directory and add that directory to includes
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in
                ${CMAKE_CURRENT_SOURCE_DIR}/src/version.h
                @ONLY)

enable_testing()


# Detect OS and print to console
if(WIN32)
    message(STATUS "Operating System: Windows")
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
    Location
    Network
    Positioning
    PrintSupport
    Qml
    Quick
    QuickWidgets
    SerialPort
    Sql
    Widgets
    REQUIRED)



# Add subdirectory for source files
add_subdirectory(src)
add_subdirectory(tests)
# Optionally, you can include other subdirectories for resources, docs, etc.
