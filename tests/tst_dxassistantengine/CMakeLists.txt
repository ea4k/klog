cmake_minimum_required(VERSION 3.28)
project(tst_dxassistantengine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Concurrent Network Sql Test Widgets)
set(CMAKE_AUTOMOC ON)

file(GLOB TEST_SOURCES "*.cpp")
list(FILTER TEST_SOURCES EXCLUDE REGEX "/moc_.*\\.cpp$")
list(FILTER TEST_SOURCES EXCLUDE REGEX "/ui_.*\\.cpp$")
list(FILTER TEST_SOURCES EXCLUDE REGEX "/qrc_.*\\.cpp$")

file(GLOB TEST_HEADERS "*.h")

add_executable(tst_dxassistantengine
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
    ../../src/dxcluster/clublogmostwanted.cpp
    ../../src/dxcluster/dxassistantengine.cpp
    ../../src/dxcluster/dxspot.cpp
    ../../src/filemanager.cpp
    ../../src/frequency.cpp
    ../../src/locator.cpp
    ../../src/qso.cpp
    ../../src/utilities.cpp
    ../../src/world.cpp

    ${TEST_HEADERS}
    ../../src/awards.h
    ../../src/dataproxy_sqlite.h
    ../../src/dxcluster/clublogmostwanted.h
    ../../src/dxcluster/dxassistantengine.h
    ../../src/dxcluster/dxspot.h
    ../../src/klogdefinitions.h
    ../../src/world.h
)

target_link_libraries(tst_dxassistantengine
    Qt6::Core
    Qt6::Network
    Qt6::Sql
    Qt6::Test
    Qt6::Widgets
    Qt6::Concurrent
)

target_compile_definitions(tst_dxassistantengine PRIVATE KLOG_TESTING)

add_test(NAME tst_dxassistantengine COMMAND tst_dxassistantengine)
set_tests_properties(tst_dxassistantengine PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
