project(klog)

set (CMAKE_BUILD_TYPE release)

find_package(KDE4 REQUIRED)

#add_definitions (-DQT3_SUPPORT -DQT3_SUPPORT_WARNINGS)

include_directories( ${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ../build )

set(klog_SRCS
    adif.cpp
    award.cpp
    awarddxcc.cpp
    awardiota.cpp
    awards.cpp
    awardwaz.cpp
    cabrillomanagement.cpp
    calls.cpp
    dxspot.cpp
    entity.cpp
    klog.cpp
    kloghamlib.cpp
    locator.cpp
    main.cpp
    prefix.cpp
    qso.cpp
    reference.cpp
    setup.cpp
    workedcalls.cpp
    world.cpp)

kde4_add_ui_files(klog_SRCS klog.ui setup.ui)

kde4_add_executable(klog ${klog_SRCS})

#target_link_libraries(klog ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KDECORE_LIBS} hamlib kdeprint kdeprint)
target_link_libraries(klog ${KDE4_KDEUI_LIBS} ${KDE4_KDECORE_LIBS} hamlib)

install(TARGETS klog DESTINATION ${BIN_INSTALL_DIR})
#install( FILES klogui.rc
#         DESTINATION  ${DATA_INSTALL_DIR}/klog )

#install(FILES  klog.desktop DESTINATION ${APPLNK_INSTALL_DIR}/Utilities)

#kde4_install_icons(${ICON_INSTALL_DIR})
