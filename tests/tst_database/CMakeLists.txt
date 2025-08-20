cmake_minimum_required(VERSION 3.31)
project(tst_database)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 Core and Test modules
find_package(Qt6 REQUIRED COMPONENTS Core Test)
# Enable AUTOMOC before add_executable
set(CMAKE_AUTOMOC ON)

# Collect test sources
file(GLOB TEST_SOURCES "*.cpp")
file(GLOB TEST_HEADERS "*.h")

# Create the test executable
add_executable(tst_database
    ${TEST_SOURCES}
        ../../src/adif.cpp
        ../../src/frequency.cpp
        ../../src/locator.cpp
        ../../src/utilities.cpp
        ../../src/world.cpp
        ../../src/dataproxy_sqlite.cpp
        ../../src/qso.cpp
        ../../src/callsign.cpp
        ../../src/database/queryexecutor.cpp
        ../../src/database/db_adif_primary_subdvisions_data.cpp
        ../../src/database.cpp


    ${TEST_HEADERS}
    ../../src/database.h
    ../../src/database/queryexecutor.h
    ../../src/database/db_adif_primary_subdvisions_data.h
    ../../src/global.h
    ../../src/adif.h
    ../../src/frequency.h
    ../../src/locator.h
    ../../src/world.h
    ../../src/dataproxy_sqlite.h
    ../../src/qso.h
    ../../src/callsign.h
    ../../src/utilities.h
)

# Link with Qt6 Core and Test
target_link_libraries(tst_database
    Qt6::Core
    Qt6::Test
    Qt6::Sql
    Qt6::Widgets
)

# Register the test with CTest
add_test(NAME tst_database COMMAND tst_database)

