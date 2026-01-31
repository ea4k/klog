cmake_minimum_required(VERSION 3.30)
project(tst_mainwindosattab)

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
add_executable(tst_mainwindosattab
    ${TEST_SOURCES}
    ../../src/inputwidgets/mainwindowsattab.cpp
    ../../src/dataproxy_sqlite.cpp
    ../../src/qso.cpp
    ../../src/qsodatacache.cpp
    ../../src/frequency.cpp
    ../../src/callsign.cpp
    ../../src/utilities.cpp
    ../../src/locator.cpp
    ../../src/database.cpp
    ../../src/database/queryexecutor.cpp
    ../../src/database/db_adif_primary_subdvisions_data.cpp
    ../../src/adif.cpp


    ${TEST_HEADERS}
    ../../src/inputwidgets/mainwindowsattab.h
    ../../src/dataproxy_sqlite.h
    ../../src/qso.h
    ../../src/qsodatacache.h
    ../../src/frequency.h
    ../../src/callsign.h
    ../../src/utilities.h
    ../../src/locator.h
    ../../src/klogdefinitions.h
    ../../src/database.h
    ../../src/database/queryexecutor.h
    ../../src/database/db_adif_primary_subdvisions_data.h
    ../../src/adif.h
)

# Enable MOC for Qt signals/slots if needed
set(CMAKE_AUTOMOC ON)

# Link with Qt6 Core and Test
target_link_libraries(tst_mainwindosattab
    Qt6::Core
    Qt6::Test
    Qt6::Sql
    Qt6::Widgets
)

# Register the test with CTest
add_test(NAME tst_mainwindosattab COMMAND tst_mainwindosattab)

