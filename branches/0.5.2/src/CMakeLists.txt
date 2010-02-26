
#include_directories( ${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ../build )

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
qt4_add_resources (klog_SRCS ../klog-resources.qrc )
kde4_add_executable(klog ${klog_SRCS})

target_link_libraries(klog ${KDE4_KDEUI_LIBS} ${KDE4_KDECORE_LIBS} hamlib)

install(TARGETS klog DESTINATION ${BIN_INSTALL_DIR})
#install( FILES klogui.rc
#         DESTINATION  ${DATA_INSTALL_DIR}/klog )

install( FILES klog.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES klogui.rc  DESTINATION  ${DATA_INSTALL_DIR}/klog )