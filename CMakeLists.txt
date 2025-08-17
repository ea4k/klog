cmake_minimum_required(VERSION 3.16)
project(KLog VERSION 2.5 LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(APP_VERSION "${PROJECT_VERSION}")
set(APP_PKGVERSION "2.5-RC1")
#set(APP_PKGVERSION "2.4.2-RC3")

# test this light line:
# find_package(Qt6 COMPONENTS Core Gui Sql Network Serialport Printsupport Charts Quickwidgets Positioning REQUIRED)
find_package(Qt6 COMPONENTS
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

# --- Hamlib support using pkg-config ---
#find_package(PkgConfig REQUIRED)
#pkg_check_modules(HAMLIB REQUIRED hamlib)
include_directories(${HAMLIB_INCLUDE_DIRS})

# Gather all sources, headers, and UI files
set(SOURCES
    src/main.cpp
    src/aboutdialog.cpp
    src/adif.cpp
    src/awardswidget.cpp
    src/callsign.cpp
    src/charts/statsfieldperbandwidget.cpp
    src/database/db_adif_primary_subdvisions_data.cpp
    src/database/queryexecutor.cpp
    src/dxcluster/dxspot.cpp
    src/dxcluster/dxcluster.cpp
    src/dxcluster/dxclusterassistant.cpp
    src/elogqrzlog.cpp
    src/eqslutilities.cpp
    src/frequency.cpp
    src/lotwutilities.cpp
    src/mainqsoentrywidget.cpp
    src/mainwindow.cpp
    src/inputwidgets/mainwindowinputqso.cpp
    src/inputwidgets/mainwindowinputcomment.cpp
    src/inputwidgets/mainwindowmydatatab.cpp
    src/inputwidgets/mainwindowinputothers.cpp
    src/inputwidgets/mainwindowinputeqsl.cpp
    src/inputwidgets/mainwindowinputqsl.cpp
    src/inputwidgets/mainwindowsattab.cpp
    src/searchmodel.cpp
    src/searchwindow.cpp
    src/setupdialog.cpp
    src/setuppages/hamlibnetworkconfigwidget.cpp
    src/setuppages/hamlibserialconfigwidget.cpp
    src/setuppages/setuppageelog.cpp
    src/setuppages/setuppagelogview.cpp
    src/setuppages/setuppagesubdivisionnew.cpp
    src/setuppages/setuppagesubdivisions.cpp
    src/widgets/adiflotwexportwidget.cpp
    src/widgets/map/mapwidget.cpp
    src/widgets/map/mapwindowwidget.cpp
    src/widgets/onlinemessagewidget.cpp
    src/widgets/showadifimportwidget.cpp
    src/widgets/showkloglogwidget.cpp
    src/logwindow.cpp
    src/filemanager.cpp
    src/fileawardmanager.cpp
    src/database.cpp
    src/dataproxy_sqlite.cpp
    src/downloadcty.cpp    
    src/locator.cpp
    src/qso.cpp
    src/awards.cpp
    src/setuppages/setuppagemisc.cpp
    src/setuppages/setuppageuserdata.cpp
    src/setuppages/setuppagedxcluster.cpp
    src/setuppages/setuppagecolors.cpp
    src/setuppages/setuppagelogs.cpp
    src/setuppages/setuppageworldeditor.cpp
    src/setuppages/setuppagelogsnew.cpp
    src/setuppages/setuppagebandmode.cpp
    src/setuppages/setuppageudp.cpp
    src/setuppages/setuppagesats.cpp
    src/setuppages/setuppagesatsnew.cpp
    src/setuppages/setuppagehamlib.cpp
    src/setuppages/setupentitydialog.cpp
    src/startwizard.cpp
    src/awarddxmarathon.cpp
    src/elogclublog.cpp
    src/softwareupdate.cpp
    src/softwareupdatedialog.cpp
    src/utilities.cpp
    src/dxccstatuswidget.cpp
    src/logmodel.cpp
    src/searchwidget.cpp
    src/infowidget.cpp
    src/showerrordialog.cpp
    src/udpserver.cpp
    src/statisticswidget.cpp
    src/updatesatsdata.cpp
    src/charts/statsgeneralchartwidget.cpp
    src/charts/statsdxccsonsatswidget.cpp
    src/charts/statsqsosperyearbarchartwidget.cpp
    src/charts/statsentitiesperyearbarchartwidget.cpp
    src/charts/statscqzperyearbarchartwidget.cpp
    src/charts/statsqsospermodebarchartwidget.cpp
    src/charts/statsqsosperdxccbarchartwidget.cpp
    src/charts/statsqsospercontinentbarchartwidget.cpp
    src/charts/statsqsosperhourbarchartwidget.cpp
    src/charts/statsqsospermonthbarchartwidget.cpp
    src/charts/statsworkedconfirmedpiechartwidget.cpp
    src/charts/statsworkedsentpiechartwidget.cpp
    src/charts/statssentconfirmedpiechartwidget.cpp
    src/charts/statsqsosperbandbarchartwidget.cpp
    src/charts/statsgridsonsatswidget.cpp
    src/hamlibclass.cpp
    src/tipsdialog.cpp
    src/updatesettings.cpp
    src/world.cpp
)

set(HEADERS
    src/setupdialog.h
    src/aboutdialog.h
    src/adif.h
    src/awarddxmarathon.h
    src/awards.h
    src/awardswidget.h
    src/callsign.h
	src/charts/statsfieldperbandwidget.h
	src/charts/statsgeneralchartwidget.h
	src/charts/statsdxccsonsatswidget.h
	src/charts/statsqsosperyearbarchartwidget.h
	src/charts/statsentitiesperyearbarchartwidget.h
	src/charts/statscqzperyearbarchartwidget.h
	src/charts/statsqsosperbandbarchartwidget.h
	src/charts/statsqsospermodebarchartwidget.h
	src/charts/statsqsosperdxccbarchartwidget.h
	src/charts/statsqsospercontinentbarchartwidget.h
	src/charts/statsqsosperhourbarchartwidget.h
	src/charts/statsqsospermonthbarchartwidget.h
	src/charts/statsworkedconfirmedpiechartwidget.h
	src/charts/statsworkedsentpiechartwidget.h
	src/charts/statsgridsonsatswidget.h
	src/charts/statssentconfirmedpiechartwidget.h
	src/database.h
	src/database/db_adif_primary_subdvisions_data.h
	src/database/queryexecutor.h
	src/dataproxy_sqlite.h
	src/downloadcty.h
	src/frequency.h
	src/dxccstatuswidget.h
	src/dxcluster/dxspot.h
	src/dxcluster/dxcluster.h
	src/dxcluster/dxclusterassistant.h
	src/elogqrzlog.h
	src/eqslutilities.h
	src/global.h
	src/hamlibclass.h
	src/inputwidgets/mainwindowinputqso.h
	src/inputwidgets/mainwindowinputcomment.h
	src/inputwidgets/mainwindowmydatatab.h
	src/inputwidgets/mainwindowinputothers.h
	src/inputwidgets/mainwindowinputeqsl.h
	src/inputwidgets/mainwindowinputqsl.h
	src/inputwidgets/mainwindowsattab.h
	src/klogdefinitions.h
	src/lotwutilities.h
	src/mainqsoentrywidget.h
	src/mainwindow.h
	src/qso.h
	src/searchmodel.h
	src/searchwindow.h
	src/logwindow.h
	src/filemanager.h
	src/fileawardmanager.h
	src/locator.h
	src/startwizard.h
	src/elogclublog.h
	src/softwareupdate.h
	src/softwareupdatedialog.h
	src/utilities.h
	src/logmodel.h
	src/searchwidget.h
	src/infowidget.h
	src/showerrordialog.h
	src/statisticswidget.h
	src/setuppages/setuppagemisc.h
	src/setuppages/hamlibnetworkconfigwidget.h
	src/setuppages/hamlibserialconfigwidget.h
	src/setuppages/setuppagelogview.h
	src/setuppages/setuppageuserdata.h
	src/setuppages/setuppagedxcluster.h
	src/setuppages/setuppagecolors.h
	src/setuppages/setuppagelogs.h
	src/setuppages/setuppageworldeditor.h
	src/setuppages/setuppagesats.h
	src/setuppages/setuppagesatsnew.h
	src/setuppages/setuppagehamlib.h
	src/setuppages/setuppagelogsnew.h
	src/setuppages/setuppagebandmode.h
	src/setuppages/setupentitydialog.h
	src/setuppages/setuppageudp.h
	src/setuppages/setuppageelog.h
	src/setuppages/setuppagesubdivisionnew.h
	src/setuppages/setuppagesubdivisions.h
	src/tipsdialog.h
	src/udpserver.h
	src/updatesatsdata.h
	src/widgets/map/mapwidget.h
	src/widgets/map/mapwindowwidget.h
	src/widgets/adiflotwexportwidget.h
	src/widgets/showkloglogwidget.h
	src/widgets/onlinemessagewidget.h
	src/widgets/showadifimportwidget.h
	src/updatesettings.h
    	src/world.h
)

set(UIS
    # ... add .ui files here, if any
    # src/klog.ui
)

set(RCCS
    # ... add .qrc files here, if any
    src/klog.qrc
)
# Version header (used by main.cpp via #include "version.h")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h @ONLY)

add_executable(klog
    ${SOURCES}
    ${HEADERS}
    ${UIS}
    ${RCCS}
)

target_include_directories(klog PRIVATE ${CMAKE_CURRENT_BINARY_DIR})


find_package(Hamlib QUIET)
if(Hamlib_FOUND)
    target_link_libraries(klog Hamlib::Hamlib)
else()
    #find_package(PkgConfig REQUIRED)
    #pkg_check_modules(HAMLIB REQUIRED hamlib)
    include_directories(${HAMLIB_INCLUDE_DIRS})
    target_link_libraries(klog ${HAMLIB_LIBRARIES})
endif()

target_link_libraries(klog
    Qt6::Charts
    Qt6::Core
    Qt6::Gui
    Qt6::Location
    Qt6::Network
    Qt6::PrintSupport
    Qt6::Positioning
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickWidgets
    Qt6::SerialPort
    Qt6::Sql
    Qt6::Widgets
    ${HAMLIB_LIBRARIES}
)
