if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    cmake_minimum_required(VERSION 3.28)
    project(tst_udpserver)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 modules: the UDP server needs Network and, through QSO and the
# database it uses, Sql, Gui and Widgets
find_package(Qt6 REQUIRED COMPONENTS Core Gui Network Sql Test Widgets)
# Enable AUTOMOC before add_executable
set(CMAKE_AUTOMOC ON)

# Collect test sources
file(GLOB TEST_SOURCES "*.cpp")
list(FILTER TEST_SOURCES EXCLUDE REGEX "/moc_.*\\.cpp$")
list(FILTER TEST_SOURCES EXCLUDE REGEX "/ui_.*\\.cpp$")
list(FILTER TEST_SOURCES EXCLUDE REGEX "/qrc_.*\\.cpp$")

file(GLOB TEST_HEADERS "*.h")

# Create the test executable
add_executable(tst_udpserver
    ${TEST_SOURCES}
    ../../src/adif.cpp
    ../../src/callsign.cpp
    ../../src/database/database.cpp
    ../../src/database/datacache.cpp
    ../../src/database/db_adif_primary_subdvisions_data.cpp
    ../../src/database/queryexecutor.cpp
    ../../src/frequency.cpp
    ../../src/locator.cpp
    ../../src/qso.cpp
    ../../src/udpserver.cpp
    ../../src/utilities.cpp

    ${TEST_HEADERS}
    ../../src/callsign.h
    ../../src/frequency.h
    ../../src/global.h
    ../../src/klogdefinitions.h
    ../../src/qso.h
    ../../src/udpserver.h
    ../../src/utilities.h
)

target_link_libraries(tst_udpserver
    Qt6::Core
    Qt6::Gui
    Qt6::Network
    Qt6::Sql
    Qt6::Test
    Qt6::Widgets
)

# Register the test with CTest
add_test(NAME tst_udpserver COMMAND tst_udpserver)
set_tests_properties(tst_udpserver PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
