cmake_minimum_required(VERSION 3.30)
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

file(GLOB_RECURSE SRC_UI  CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.ui")
file(GLOB_RECURSE SRC_QRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.qrc")

# (Optional, IDE nicety)





#file(GLOB KLOG_UIS "*.ui")
set(KLOG_SOURCES
   aboutdialog.cpp
   adif.cpp
   awarddxmarathon.cpp
   awards.cpp
   awardswidget.cpp
   callsign.cpp
   database.cpp
   dataproxy_sqlite.cpp
   downloadcty.cpp
   dxccstatuswidget.cpp
   elogclublog.cpp
   elogqrzlog.cpp
   eqslutilities.cpp
   fileawardmanager.cpp
   filemanager.cpp
   frequency.cpp
   hamlibclass.cpp
   infowidget.cpp
   locator.cpp
   logmodel.cpp
   logwindow.cpp
   lotwutilities.cpp
   main.cpp
   mainqsoentrywidget.cpp
   mainwindow.cpp
   qso.cpp
   searchmodel.cpp
   searchwidget.cpp
   searchwindow.cpp
   setupdialog.cpp
   showerrordialog.cpp
   softwareupdate.cpp
   softwareupdatedialog.cpp
   startwizard.cpp
   statisticswidget.cpp
   tipsdialog.cpp
   udpserver.cpp
   updatesatsdata.cpp
   updatesettings.cpp
   utilities.cpp
   world.cpp
   charts/statscqzperyearbarchartwidget.cpp
   charts/statsdxccsonsatswidget.cpp
   charts/statsentitiesperyearbarchartwidget.cpp
   charts/statsfieldperbandwidget.cpp
   charts/statsgeneralchartwidget.cpp
   charts/statsgridsonsatswidget.cpp
   charts/statsqsosperbandbarchartwidget.cpp
   charts/statsqsospercontinentbarchartwidget.cpp
   charts/statsqsosperdxccbarchartwidget.cpp
   charts/statsqsosperhourbarchartwidget.cpp
   charts/statsqsospermodebarchartwidget.cpp
   charts/statsqsospermonthbarchartwidget.cpp
   charts/statsqsosperyearbarchartwidget.cpp
   charts/statssentconfirmedpiechartwidget.cpp
   charts/statsworkedconfirmedpiechartwidget.cpp
   charts/statsworkedsentpiechartwidget.cpp
   database/db_adif_primary_subdvisions_data.cpp
   database/queryexecutor.cpp
   dxcluster/dxclusterassistant.cpp
   dxcluster/dxcluster.cpp
   dxcluster/dxspot.cpp
   inputwidgets/mainwindowinputcomment.cpp
   inputwidgets/mainwindowinputeqsl.cpp
   inputwidgets/mainwindowinputothers.cpp
   inputwidgets/mainwindowinputqsl.cpp
   inputwidgets/mainwindowinputqso.cpp
   inputwidgets/mainwindowmydatatab.cpp
   inputwidgets/mainwindowsattab.cpp
   setuppages/hamlibnetworkconfigwidget.cpp
   setuppages/hamlibserialconfigwidget.cpp
   setuppages/setupentitydialog.cpp
   setuppages/setuppagebandmode.cpp
   setuppages/setuppagecolors.cpp
   setuppages/setuppagedxcluster.cpp
   setuppages/setuppageelog.cpp
   setuppages/setuppagehamlib.cpp
   setuppages/setuppagelogs.cpp
   setuppages/setuppagelogsnew.cpp
   setuppages/setuppagelogview.cpp
   setuppages/setuppagemisc.cpp
   setuppages/setuppagesats.cpp
   setuppages/setuppagesatsnew.cpp
   setuppages/setuppagesubdivisionnew.cpp
   setuppages/setuppagesubdivisions.cpp
   setuppages/setuppageudp.cpp
   setuppages/setuppageuserdata.cpp
   setuppages/setuppageworldeditor.cpp
   widgets/adiflotwexportwidget.cpp
   widgets/onlinemessagewidget.cpp
   widgets/showadifimportwidget.cpp
   widgets/showkloglogwidget.cpp
   widgets/map/mapwidget.cpp
   widgets/map/mapwindowwidget.cpp
)

set(KLOG_HEADERS
   aboutdialog.h
   adif.h
   awarddxmarathon.h
   awards.h
   awardswidget.h
   callsign.h
   database.h
   dataproxy_sqlite.h
   downloadcty.h
   dxccstatuswidget.h
   elogclublog.h
   elogqrzlog.h
   eqslutilities.h
   fileawardmanager.h
   filemanager.h
   frequency.h
   global.h
   hamlibclass.h
   infowidget.h
   klogdefinitions.h
   locator.h
   logmodel.h
   logwindow.h
   lotwutilities.h
   mainqsoentrywidget.h
   mainwindow.h
   qso.h
   searchmodel.h
   searchwidget.h
   searchwindow.h
   setupdialog.h
   showerrordialog.h
   softwareupdatedialog.h
   softwareupdate.h
   startwizard.h
   statisticswidget.h
   tipsdialog.h
   udpserver.h
   updatesatsdata.h
   updatesettings.h
   utilities.h
   world.h
   global.h
   klogdefinitions.h
   charts/statscqzperyearbarchartwidget.h
   charts/statsdxccsonsatswidget.h
   charts/statsentitiesperyearbarchartwidget.h
   charts/statsfieldperbandwidget.h
   charts/statsgeneralchartwidget.h
   charts/statsgridsonsatswidget.h
   charts/statsqsosperbandbarchartwidget.h
   charts/statsqsospercontinentbarchartwidget.h
   charts/statsqsosperdxccbarchartwidget.h
   charts/statsqsosperhourbarchartwidget.h
   charts/statsqsospermodebarchartwidget.h
   charts/statsqsospermonthbarchartwidget.h
   charts/statsqsosperyearbarchartwidget.h
   charts/statssentconfirmedpiechartwidget.h
   charts/statsworkedconfirmedpiechartwidget.h
   charts/statsworkedsentpiechartwidget.h
   database/db_adif_primary_subdvisions_data.h
   database/queryexecutor.h
   dxcluster/dxclusterassistant.h
   dxcluster/dxcluster.h
   dxcluster/dxspot.h
   inputwidgets/mainwindowinputcomment.h
   inputwidgets/mainwindowinputeqsl.h
   inputwidgets/mainwindowinputothers.h
   inputwidgets/mainwindowinputqsl.h
   inputwidgets/mainwindowinputqso.h
   inputwidgets/mainwindowmydatatab.h
   inputwidgets/mainwindowsattab.h
   setuppages/hamlibnetworkconfigwidget.h
   setuppages/hamlibserialconfigwidget.h
   setuppages/setupentitydialog.h
   setuppages/setuppagebandmode.h
   setuppages/setuppagecolors.h
   setuppages/setuppagedxcluster.h
   setuppages/setuppageelog.h
   setuppages/setuppagehamlib.h
   setuppages/setuppagelogs.h
   setuppages/setuppagelogsnew.h
   setuppages/setuppagelogview.h
   setuppages/setuppagemisc.h
   setuppages/setuppagesats.h
   setuppages/setuppagesatsnew.h
   setuppages/setuppagesubdivisionnew.h
   setuppages/setuppagesubdivisions.h
   setuppages/setuppageudp.h
   setuppages/setuppageuserdata.h
   setuppages/setuppageworldeditor.h
   widgets/adiflotwexportwidget.h
   widgets/onlinemessagewidget.h
   widgets/showadifimportwidget.h
   widgets/showkloglogwidget.h
   widgets/map/mapwidget.h
   widgets/map/mapwindowwidget.h
)

# Add executable

add_executable(klog
    ${KLOG_SOURCES}
    ${KLOG_HEADERS} 
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


# Prefer a config package if available (e.g., Homebrew/macOS or user-provided)
find_package (Hamlib REQUIRED)
#find_package(Hamlib CONFIG QUIET)
if(Hamlib_FOUND)
    set(_hamlib_found TRUE)
endif()

# Try module mode (uses cmake/FindHamlib.cmake if present)
if(NOT _hamlib_found)
    find_package(Hamlib MODULE QUIET)
    if(Hamlib_FOUND)
        set(_hamlib_found TRUE)
    endif()
endif()

# Linux-only fallback to pkg-config
if(NOT _hamlib_found AND UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(HAMLIB REQUIRED IMPORTED_TARGET hamlib)
    set(_hamlib_found TRUE)
endif()

# Link to whichever method was found
if(TARGET Hamlib::Hamlib)
    target_link_libraries(klog PRIVATE Hamlib::Hamlib)
elseif(TARGET PkgConfig::HAMLIB)
    target_link_libraries(klog PRIVATE PkgConfig::HAMLIB)
elseif(HAMLIB_LIBRARIES)
    target_include_directories(klog PRIVATE ${HAMLIB_INCLUDE_DIRS})
    target_link_libraries(klog PRIVATE ${HAMLIB_LIBRARIES})
else()
    message(FATAL_ERROR
        "Hamlib not found.\n"
        "Linux: sudo apt install libhamlib-dev (or equivalent)\n"
        "macOS: brew install hamlib (then set Hamlib_DIR or use FindHamlib.cmake)\n"
        "Windows: install Hamlib and set Hamlib_DIR to its CMake package or provide libs/headers."
    )
endif()


# Other platform-specific stuff (icons, etc)
if(WIN32)
    # Add Windows resources if any (e.g., icons)
elseif(APPLE)
    # Set MacOS bundle properties if any
endif()
