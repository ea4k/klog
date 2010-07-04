project(klog)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set (VERSION 0.5.6)

find_package(KDE4 REQUIRED)
include (KDE4Defaults)
find_package(Hamlib REQUIRED)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${HAMLIB_INCLUDE_DIR} )

add_subdirectory( src )
add_subdirectory( icons )
add_subdirectory( po )


