cmake_minimum_required(VERSION 3.10)
project(KLog)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)


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
