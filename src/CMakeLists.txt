cmake_minimum_required(VERSION 3.16)
project(KLog VERSION 2.5 LANGUAGES CXX)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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

# Recursively gather sources/resources under src/
file(GLOB_RECURSE SRC_CPP CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE SRC_UI  CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.ui")
file(GLOB_RECURSE SRC_QRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.qrc")

# (Optional, IDE nicety)
file(GLOB_RECURSE SRC_HDR CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

# Source files - you may need to adjust/add more sources here based on src.pro
#file(GLOB KLOG_SOURCES
#    "*.cpp"
#)
#file(GLOB KLOG_HEADERS
#    "*.h"
#)
##file(GLOB KLOG_RESOURCES
#    "*.qrc"
#)

# UI files
#file(GLOB KLOG_UIS "*.ui")

# Add executable

add_executable(klog
    ${SRC_CPP}
    ${SRC_HDR}   # optional
    ${SRC_UI}
    ${SRC_QRC}
)



# Ensure the generated version.h is visible
target_include_directories(klog PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# Qt MOC/UIC/RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Link Qt6 libraries
target_link_libraries(klog
  PRIVATE
    Qt6::Charts
    Qt6::Core
    Qt6::Gui
    Qt6::Location
    Qt6::Network
    Qt6::Positioning
    Qt6::PrintSupport
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickWidgets
    Qt6::SerialPort
    Qt6::Sql
    Qt6::Widgets
)


# If your FindHamlib defines an imported target:

if(TARGET Hamlib::Hamlib)
  target_link_libraries(klog PRIVATE Hamlib::Hamlib)
elseif(TARGET PkgConfig::HAMLIB)
  target_link_libraries(klog PRIVATE PkgConfig::HAMLIB)
elseif(HAMLIB_LIBRARIES)
  target_include_directories(klog PRIVATE ${HAMLIB_INCLUDE_DIRS})
  target_link_libraries(klog PRIVATE ${HAMLIB_LIBRARIES})
else()
  message(FATAL_ERROR "Hamlib not found. Install libhamlib-dev or provide Hamlib via cmake/FindHamlib.cmake.")
endif()


# Other platform-specific stuff (icons, etc)
if(WIN32)
    # Add Windows resources if any (e.g., icons)
elseif(APPLE)
    # Set MacOS bundle properties if any
endif()
