set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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

set(KLOG_SOURCES
   aboutdialog.cpp
   adif.cpp
   awarddxmarathon.cpp
   awards.cpp
   awardswidget.cpp
   callsign.cpp
   database/database.cpp
   database/datacache.cpp
   database/db_adif_primary_subdvisions_data.cpp
   database/queryexecutor.cpp
   dataproxy_sqlite.cpp
   downloadcty.cpp
   dxccstatuswidget.cpp
   elog/elogclublog.cpp
   elog/lotwutilities.cpp
   elog/elogqrzlog.cpp
   elog/eqslutilities.cpp
   fileawardmanager.cpp
   filemanager.cpp
   frequency.cpp
   hamlibclass.cpp
   infowidget.cpp
   locator.cpp
   logmodel.cpp
   logwindow.cpp
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
   widgets/map/locatorinfoprovider.cpp
   klog.rc
)

set(KLOG_HEADERS
   aboutdialog.h
   adif.h
   awarddxmarathon.h
   awards.h
   awardswidget.h
   callsign.h
   database/database.h
   database/datacache.h
   database/db_adif_primary_subdvisions_data.h
   database/queryexecutor.h
   dataproxy_sqlite.h
   downloadcty.h
   dxccstatuswidget.h
   elog/elogclublog.h
   elog/lotwutilities.h
   elog/elogqrzlog.h
   elog/eqslutilities.h
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
   widgets/map/locatorinfoprovider.h
)

set(KLOG_DOC
   ../AUTHORS
   ../Changelog
   ../COPYING
   ../INSTALL-es.txt
   ../INSTALL-linux
   ../INSTALL.txt
   ../NEWS
   ../README
   ../README-DEVEL
   ../TODO
   ../TROUBLESHOUTING
)

# Documentation files relevant only on Linux/generic installs
# (excluded from macOS bundle and Windows installer)
set(KLOG_DOC_LINUX
   ../INSTALL-es.txt
   ../INSTALL-linux
)

set(KLOG_MAN
    klog.1
)

find_package(Qt6 QUIET COMPONENTS LinguistTools)

# Translation files
set(KLOG_TS_FILES
    translations/klog_ca.ts
    translations/klog_cs.ts
    translations/klog_da.ts
    translations/klog_de.ts
    translations/klog_en.ts
    translations/klog_es.ts
    translations/klog_fi.ts
    translations/klog_fr.ts
    translations/klog_hr.ts
    translations/klog_it.ts
    translations/klog_ja.ts
    translations/klog_lv.ts
    translations/klog_nl.ts
    translations/klog_pl.ts
    translations/klog_ru.ts
    translations/klog_uk.ts
)

# Qt MOC/UIC/RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Add executable
add_executable(klog WIN32 MACOSX_BUNDLE
    ${KLOG_SOURCES}
    ${KLOG_HEADERS}
    ${SRC_UI}
    ${SRC_QRC}
    ${KLOG_DOC}
    ${KLOG_MAN}
    ${KLOG_TS_FILES}
)

source_group("Documentation" FILES ${KLOG_DOC} ${KLOG_DOC_LINUX})
source_group("Man Pages"     FILES ${KLOG_MAN})
source_group("Translations"  FILES ${KLOG_TS_FILES})
source_group("UI Forms"      FILES ${SRC_UI})
source_group("Resources"     FILES ${SRC_QRC})

# Ensure the generated version.h is visible
target_include_directories(klog PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

if(Qt6LinguistTools_FOUND)
    qt_add_translations(klog
        TS_FILES ${KLOG_TS_FILES}
        QM_FILES_OUTPUT_VARIABLE QM_FILES
    )
else()
    message(STATUS "Qt6 LinguistTools not found - skipping translations (install qt6-tools-dev qt6-l10n-tools)")
endif()

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
find_package(Hamlib REQUIRED)
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

if(APPLE)
    install(TARGETS klog BUNDLE DESTINATION .)
    set_source_files_properties(${KLOG_DOC} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set_source_files_properties(${QM_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/translations)
    target_sources(klog PRIVATE ${QM_FILES})
    set_target_properties(klog PROPERTIES
           # OUTPUT_NAME controls the .app bundle name visible in the DMG and Finder.
           # Without this the bundle inherits the target name "klog" (lowercase).
           OUTPUT_NAME                           "KLog"
           MACOSX_BUNDLE_BUNDLE_NAME             "KLog"
           MACOSX_BUNDLE_ICON_FILE               klog.icns
           MACOSX_BUNDLE_BUNDLE_VERSION          "${APP_PKGVERSION}"
           MACOSX_BUNDLE_SHORT_VERSION_STRING    "${APP_PKGVERSION}"
           MACOSX_BUNDLE_GUI_IDENTIFIER          "es.ea4k.klog"
       )
    set_source_files_properties(klog.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    target_sources(klog PRIVATE klog.icns)

elseif(WIN32)
   # --- Windows installer ---
   # Binary goes to the root installation folder alongside docs and translations.
   install(TARGETS klog
       BUNDLE  DESTINATION .
       RUNTIME DESTINATION .
   )
   install(FILES ${KLOG_DOC} DESTINATION .)
   install(FILES ${QM_FILES} DESTINATION translations)
else()
   # =========================================================================
    # Linux / Unix install — FHS + Debian Policy compliant
    # =========================================================================

    # freedesktop integration files — only defined and used on Linux
    set(KLOG_DESKTOP  klog.desktop)
    set(KLOG_ICON     klog.png)
    set(KLOG_METAINFO klog.metainfo.xml)

    # Binary: /usr/bin/klog
    install(TARGETS klog
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    # Documentation: /usr/share/doc/klog/
    # Debian Policy §12.3: all package docs go under /usr/share/doc/<package>.
    # Only install docs relevant on Linux; exclude Windows/macOS install guides.
    install(FILES ${KLOG_DOC} ${KLOG_DOC_LINUX}
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
    )

    # Man page: /usr/share/man/man1/
    install(FILES ${KLOG_MAN}
        DESTINATION ${CMAKE_INSTALL_MANDIR}/man1
    )

    # Translations (arch-independent runtime data): /usr/share/klog/translations/
    # FHS: /usr/share/<package> is for arch-independent data, NOT documentation.
    if(QM_FILES)
        install(FILES ${QM_FILES}
            DESTINATION ${CMAKE_INSTALL_DATADIR}/klog/translations
        )
    endif()

    # Desktop entry: /usr/share/applications/klog.desktop
    # Required by Debian Policy §9.6 and freedesktop.org for desktop integration.
    install(FILES ${KLOG_DESKTOP}
        DESTINATION ${CMAKE_INSTALL_DATADIR}/applications
    )

    # Application icon: /usr/share/pixmaps/klog.png
    # Legacy path; compatible with all desktop environments.
    # If a scalable SVG is added in the future, also install to:
    #   ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps/klog.svg
    install(FILES ${KLOG_ICON}
        DESTINATION ${CMAKE_INSTALL_DATADIR}/pixmaps
    )

    # AppStream metainfo: /usr/share/metainfo/klog.metainfo.xml
    # Required to appear in GNOME Software, KDE Discover, and future Debian
    # software catalogs. Debian Package Tracker warning: no-metainfo (active).
    install(FILES ${KLOG_METAINFO}
        DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo
    )
endif()

# Other platform-specific stuff (icons, etc)
if(WIN32)
    # Add Windows resources if any (e.g., icons)
elseif(APPLE)
    # Set MacOS bundle properties if any
endif()
