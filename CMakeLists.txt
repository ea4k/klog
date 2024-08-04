cmake_minimum_required(VERSION 3.16)
project (KLog
    VERSION 1.0
    LANGUAGES C CXX)

set (PROJECT_DESCRIPTION "KLog - The free hamradio logging program")
set (CMAKE_PROJECT_DESCRIPTION ${PROJECT_DESCRIPTION})

#Some code of this cmakelists.txt has been copied from WSJTX, thank you!
set (PROJECT_ARCHITECTURE "${CMAKE_SYSTEM_PROCESSOR}")
if (NOT PROJECT_ARCHITECTURE)
  # This is supposed to happen already on Windows
  if (CMAKE_SIZEOF_VOID_P MATCHES 8)
    set (PROJECT_ARCHITECTURE "x64")
  else ()
    set (PROJECT_ARCHITECTURE "$ENV{PROCESSOR_ARCHITECTURE}")
  endif ()
endif ()
message (STATUS "******************************************************")
message (STATUS "Building for for: ${CMAKE_SYSTEM_NAME}-${PROJECT_ARCHITECTURE}")
message (STATUS "******************************************************")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Charts Gui Network Positioning PrintSupport QuickWidgets SerialPort Sql Test Widgets)

qt_standard_project_setup()


add_subdirectory(src)
