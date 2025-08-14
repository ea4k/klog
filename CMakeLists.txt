cmake_minimum_required(VERSION 3.16)
project(KLog VERSION 2.4.2 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(APP_VERSION "${PROJECT_VERSION}")
set(APP_PKGVERSION "2.4.2-RC3")
# set(APP_PKGVERSION "2.4.2-RC3")

# Make custom find modules available (for Hamlib fallback)
list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Qt 6
find_package(Qt6 COMPONENTS Charts Core Gui Qml Widgets Sql Location Network Positioning PrintSupport QuickWidgets SerialPort REQUIRED)

# Collect sources/headers/resources
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

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)

add_executable(klog
    ${SOURCES}
    ${HEADERS}
    ${UIS}
    ${RCCS}
)

target_include_directories(klog PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# -------------------------
# Hamlib discovery (portable)
# -------------------------
# 1) Prefer a CMake config package (e.g., from vcpkg or a system install shipping HamlibConfig.cmake)
find_package(Hamlib CONFIG QUIET)

# 2) If not found, try a simple module-mode finder (provided in cmake/FindHamlib.cmake)
if(NOT Hamlib_FOUND)
    find_package(Hamlib QUIET)
endif()

# 3) If still not found, try pkg-config (typical on Linux/MSYS2)
set(_HAMLIB_LIBS "")
if(Hamlib_FOUND)
    target_link_libraries(klog PRIVATE Hamlib::Hamlib)
else()
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(HAMLIB hamlib)
    endif()

    if(HAMLIB_FOUND)
        target_include_directories(klog PRIVATE ${HAMLIB_INCLUDE_DIRS})
        list(APPEND _HAMLIB_LIBS ${HAMLIB_LIBRARIES})
    else()
        message(FATAL_ERROR
            "Hamlib not found.\n"
            "Provide Hamlib via one of:\n"
            " - A CMake package (set Hamlib_DIR or CMAKE_PREFIX_PATH), or\n"
            " - pkg-config (ensure hamlib.pc is on PKG_CONFIG_PATH), or\n"
            " - vcpkg toolchain. See DEVELOPMENT docs."
        )
    endif()
endif()

# Link Qt and Hamlib
target_link_libraries(klog
    PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
    Qt6::Network
    Qt6::SerialPort
    Qt6::PrintSupport
    Qt6::Charts
    Qt6::QuickWidgets
    Qt6::Positioning
    ${_HAMLIB_LIBS}
)
