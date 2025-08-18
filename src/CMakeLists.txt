cmake_minimum_required(VERSION 3.16)
project(KLog VERSION 2.5 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Platform-specific settings
if(WIN32)
    add_definitions(-DWIN32)
elseif(APPLE)
    add_definitions(-DOS_MAC)
else()
    add_definitions(-DLINUX)
endif()

# Source files - you may need to adjust/add more sources here based on src.pro
file(GLOB KLOG_SOURCES
    "*.cpp"
)
file(GLOB KLOG_HEADERS
    "*.h"
)
file(GLOB KLOG_RESOURCES
    "*.qrc"
)

# UI files
file(GLOB KLOG_UIS "*.ui")

# Add executable
add_executable(KLog
    ${KLOG_SOURCES}
    ${KLOG_HEADERS}
    ${KLOG_UIS}
)

# Qt MOC/UIC/RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Link Qt6 libraries
target_link_libraries(KLog
    Qt6::Core
    Qt6::Gui
    Qt6::Network
    Qt6::PrintSupport
    Qt6::Positioning
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickWidgets
    Qt6::SerialPort
    Qt6::Sql
    Qt6::Widgets
    Qt6::Charts
    Qt6::Location
)

# Other platform-specific stuff (icons, etc)
if(WIN32)
    # Add Windows resources if any (e.g., icons)
elseif(APPLE)
    # Set MacOS bundle properties if any
endif()
