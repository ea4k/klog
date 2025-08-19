cmake_minimum_required(VERSION 3.16)
project(tst_filemanager)

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
add_executable(tst_filemanager
    ${TEST_SOURCES}
    ../../src/adif.cpp
    ../../src/database/db_adif_primary_subdvisions_data.cpp
    ../../src/awards.cpp
    ../../src/awarddxmarathon.cpp
    ../../src/database.cpp
    ../../src/callsign.cpp
    ../../src/dataproxy_sqlite.cpp
    ../../src/frequency.cpp
    ../../src/locator.cpp
    ../../src/database/queryexecutor.cpp
    ../../src/qso.cpp
    ../../src/utilities.cpp
    ../../src/world.cpp
    ../../src/filemanager.cpp

    ${TEST_HEADERS}
    ../../src/adif.h
    ../../src/database/db_adif_primary_subdvisions_data.h
    ../../src/awards.h
    ../../src/awarddxmarathon.h
    ../../src/database.h
    ../../src/callsign.h
    ../../src/dataproxy_sqlite.h
    ../../src/frequency.h
    ../../src/locator.h
    ../../src/database/queryexecutor.h
    ../../src/qso.h
    ../../src/utilities.h
    ../../src/world.h
    ../../src/filemanager.h
)

# Enable MOC for Qt signals/slots if needed
set(CMAKE_AUTOMOC ON)

# Link with Qt6 Core and Test
target_link_libraries(tst_filemanager
    Qt6::Core
    Qt6::Test
    Qt6::Sql
    Qt6::Widgets
)

# Register the test with CTest
add_test(NAME tst_filemanager COMMAND tst_filemanager)

