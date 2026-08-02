if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    cmake_minimum_required(VERSION 3.30)
    project(tst_udpclient)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the Qt6 modules needed by the client and its dependencies
find_package(Qt6 REQUIRED COMPONENTS Core Concurrent Network Sql Test Widgets)
# Enable AUTOMOC before add_executable
set(CMAKE_AUTOMOC ON)

# Collect test sources
file(GLOB TEST_SOURCES "*.cpp")
file(GLOB TEST_HEADERS "*.h")

# Create the test executable
add_executable(tst_udpclient
    ${TEST_SOURCES}
    ../../src/udpclient.cpp
    ../../src/qso.cpp
    ../../src/adif.cpp
    ../../src/callsign.cpp
    ../../src/frequency.cpp
    ../../src/locator.cpp
    ../../src/utilities.cpp
    ../../src/database/database.cpp
    ../../src/database/datacache.cpp
    ../../src/database/db_adif_primary_subdvisions_data.cpp
    ../../src/database/queryexecutor.cpp
    ../../src/dataproxy_sqlite.cpp

    ${TEST_HEADERS}
    ../../src/udpclient.h
    ../../src/qso.h
    ../../src/adif.h
    ../../src/callsign.h
    ../../src/frequency.h
    ../../src/global.h
    ../../src/klogdefinitions.h
    ../../src/locator.h
    ../../src/utilities.h
    ../../src/database/database.h
    ../../src/database/datacache.h
    ../../src/database/db_adif_primary_subdvisions_data.h
    ../../src/database/queryexecutor.h
    ../../src/dataproxy_sqlite.h
)

# Link with Qt6 Core, Network and Test
target_link_libraries(tst_udpclient
    Qt6::Core
    Qt6::Concurrent
    Qt6::Network
    Qt6::Test
    Qt6::Sql
    Qt6::Widgets
)

# Register the test with CTest
add_test(NAME tst_udpclient COMMAND tst_udpclient)
set_tests_properties(tst_udpclient PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
