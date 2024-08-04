cmake_minimum_required(VERSION 3.16)
project(KLog VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Charts Gui Network Positioning PrintSupport QuickWidgets SerialPort Sql Test Widgets)

qt_standard_project_setup()


add_subdirectory(src)
