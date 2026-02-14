cmake_minimum_required(VERSION 3.30)
project(tst_qsodatacache)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 Core and Test modules
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Test Sql)

# Enable AUTOMOC before add_executable
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORRC ON)

qt_add_resources(tst_qsodatacache "resources.qrc")

# Collect test sources
file(GLOB TEST_SOURCES "*.cpp")
file(GLOB TEST_HEADERS "*.h")

# Create the test executable
add_executable(tst_qsodatacache
    ${TEST_SOURCES}
    ../../src/callsign.cpp
    ../../src/database.cpp
    ../../src/database/datacache.cpp
    ../../src/database/db_adif_primary_subdvisions_data.cpp
    ../../src/database/queryexecutor.cpp
    ../../src/frequency.h
    ../../src/locator.h
    ../../src/qsodatacache.cpp
    ../../src/utilities.cpp

    ${TEST_HEADERS}
    ../../src/callsign.h
    ../../src/database.h
    ../../src/database/datacache.h
    ../../src/database/db_adif_primary_subdvisions_data.h 
    ../../src/database/queryexecutor.h
    ../../src/frequency.cpp
    ../../src/locator.cpp
    ../../src/qsodatacache.h    
    ../../src/utilities.h
)

# Link with Qt6 Core and Test
target_link_libraries(tst_qsodatacache
    Qt6::Core
    Qt6::Test
    Qt6::Sql
    Qt6::Widgets
)

# Register the test with CTest
add_test(NAME tst_qsodatacache COMMAND tst_qsodatacache)

