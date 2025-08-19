cmake_minimum_required(VERSION 3.16)
project(tst_qso)

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
add_executable(tst_qso
    ${TEST_SOURCES}
    ../../src/database.cpp
    ../../src/database/db_adif_primary_subdvisions_data.cpp
    ../../src/database/queryexecutor.cpp
    ../../src/utilities.cpp
    ../../src/callsign.cpp
    ../../src/frequency.cpp
    ../../src/locator.cpp
    ../../src/qso.cpp
    ../../src/adif.cpp

    ${TEST_HEADERS}
    ../../src/database.h
    ../../src/database/db_adif_primary_subdvisions_data.h
    ../../src/database/queryexecutor.h
    ../../src/utilities.h
    ../../src/callsign.h
    ../../src/frequency.h
    ../../src/global.h
    ../../src/klogdefinitions.h
    ../../src/locator.h
    ../../src/qso.h    
    ../../src/adif.h
)

# Enable MOC for Qt signals/slots if needed
set(CMAKE_AUTOMOC ON)

# Link with Qt6 Core and Test
target_link_libraries(tst_qso
    Qt6::Core
    Qt6::Test
    Qt6::Sql
)

# Register the test with CTest
add_test(NAME tst_qso COMMAND tst_qso)

