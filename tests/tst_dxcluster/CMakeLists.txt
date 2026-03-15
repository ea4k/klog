if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    cmake_minimum_required(VERSION 3.28)
    project(tst_dxcluster)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network Test Widgets)
set(CMAKE_AUTOMOC ON)

file(GLOB TEST_SOURCES "*.cpp")
list(FILTER TEST_SOURCES EXCLUDE REGEX "/moc_.*\\.cpp$")
list(FILTER TEST_SOURCES EXCLUDE REGEX "/ui_.*\\.cpp$")
list(FILTER TEST_SOURCES EXCLUDE REGEX "/qrc_.*\\.cpp$")

file(GLOB TEST_HEADERS "*.h")

add_executable(tst_dxcluster
    ${TEST_SOURCES}
    ../../src/adif.cpp
    ../../src/awards.cpp
    ../../src/awarddxmarathon.cpp
    ../../src/callsign.cpp
    ../../src/database/datacache.cpp
    ../../src/database/db_adif_primary_subdvisions_data.cpp
    ../../src/database/database.cpp
    ../../src/database/queryexecutor.cpp
    ../../src/dataproxy_sqlite.cpp
    ../../src/dxcluster/dxcluster.cpp
    ../../src/dxcluster/dxspot.cpp
    ../../src/filemanager.cpp
    ../../src/frequency.cpp
    ../../src/locator.cpp
    ../../src/qso.cpp
    ../../src/qsodatacache.cpp
    ../../src/utilities.cpp
    ../../src/world.cpp

    ${TEST_HEADERS}
    ../../src/awards.h
    ../../src/awarddxmarathon.h
    ../../src/callsign.h
    ../../src/database/datacache.h
    ../../src/database/db_adif_primary_subdvisions_data.h
    ../../src/database/database.h
    ../../src/database/queryexecutor.h
    ../../src/dataproxy_sqlite.h
    ../../src/dxcluster/dxcluster.h
    ../../src/dxcluster/dxspot.h
    ../../src/filemanager.h
    ../../src/frequency.h
    ../../src/klogdefinitions.h
    ../../src/locator.h
    ../../src/qso.h
    ../../src/qsodatacache.h
    ../../src/utilities.h
    ../../src/world.h
)

target_link_libraries(tst_dxcluster
    Qt6::Core
    Qt6::Network
    Qt6::Sql
    Qt6::Test
    Qt6::Widgets
)

add_test(NAME tst_dxcluster COMMAND tst_dxcluster)
