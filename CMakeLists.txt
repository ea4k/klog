project(klog)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set (VERSION 0.5.2)

find_package(KDE4 REQUIRED)
include (KDE4Defaults)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

add_subdirectory( src )
add_subdirectory( icons )
add_subdirectory( po )


