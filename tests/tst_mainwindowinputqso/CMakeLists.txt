cmake_minimum_required(VERSION 3.16)
project(tst_mainwindowinputqso)

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
add_executable(tst_mainwindowinputqso
    ${TEST_SOURCES}
    ../../src/inputwidgets/mainwindowinputqso.cpp    
    ../../src/dataproxy_sqlite.cpp
    ../../src/database.cpp
    ../../src/database/queryexecutor.cpp
    ../../src/database/db_adif_primary_subdvisions_data.cpp
    ../../src/frequency.cpp
    ../../src/qso.cpp
    ../../src/callsign.cpp
    ../../src/utilities.cpp
    ../../src/locator.cpp
    ../../src/adif.cpp

    ${TEST_HEADERS}
    ../../src/inputwidgets/mainwindowinputqso.h
    ../../src/dataproxy_sqlite.h
    ../../src/database.h
    ../../src/database/queryexecutor.h
    ../../src/database/db_adif_primary_subdvisions_data.h
    ../../src/frequency.h
    ../../src/qso.h
    ../../src/callsign.h
    ../../src/utilities.h
    ../../src/locator.h
    ../../src/adif.h
)

# Enable MOC for Qt signals/slots if needed
set(CMAKE_AUTOMOC ON)

# Link with Qt6 Core and Test
target_link_libraries(tst_mainwindowinputqso
    Qt6::Core
    Qt6::Test
    Qt6::Sql
    Qt6::Widgets
)

# Register the test with CTest
add_test(NAME tst_mainwindowinputqso COMMAND tst_mainwindowinputqso)

