cmake_minimum_required(VERSION 3.30)
project(tst_world)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 Core and Test modules
find_package(Qt6 REQUIRED COMPONENTS Core Test Sql)
# Enable AUTOMOC before add_executable
set(CMAKE_AUTOMOC ON)

# Collect test sources
file(GLOB TEST_SOURCES "*.cpp")
file(GLOB TEST_HEADERS "*.h")

# Create the test executable
add_executable(tst_world
    ${TEST_SOURCES}
    ../../src/world.cpp
    ../../src/dataproxy_sqlite.cpp
    ../../src/qso.cpp
    ../../src/qsodatacache.cpp
    ../../src/callsign.cpp
    ../../src/utilities.cpp
    ../../src/database/database.cpp
    ../../src/database/datacache.cpp
    ../../src/database/queryexecutor.cpp
    ../../src/database/db_adif_primary_subdvisions_data.cpp
    ../../src/frequency.cpp
    ../../src/locator.cpp
    ../../src/adif.cpp

    ${TEST_HEADERS}
    ../../src/world.h
    ../../src/dataproxy_sqlite.h
    ../../src/locator.h
    ../../src/utilities.h
    ../../src/database/database.h
    ../../src/database/datacache.h
    ../../src/database/queryexecutor.h
    ../../src/database/db_adif_primary_subdvisions_data.h
    ../../src/frequency.h
    ../../src/qso.h
    ../../src/qsodatacache.h
    ../../src/callsign.h
    ../../src/adif.h
)

# Link with Qt6 Core and Test
target_link_libraries(tst_world
    Qt6::Core
    Qt6::Test
    Qt6::Sql
    Qt6::Widgets
)

# Register the test with CTest
add_test(NAME tst_world COMMAND tst_world)

