cmake_minimum_required(VERSION 3.16)
project(tst_logwindow)

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
add_executable(tst_logwindow
    ${TEST_SOURCES}
    ../../src/adif.cpp
    ../../src/awards.cpp
    ../../src/awarddxmarathon.cpp
    ../../src/callsign.cpp
    ../../src/database/queryexecutor.cpp
    ../../src/database/db_adif_primary_subdvisions_data.cpp
    ../../src/frequency.cpp
    ../../src/database.cpp
    ../../src/dataproxy_sqlite.cpp
    ../../src/filemanager.cpp
    ../../src/locator.cpp
    ../../src/logmodel.cpp
    ../../src/logwindow.cpp
    ../../src/qso.cpp
    ../../src/world.cpp
    ../../src/utilities.cpp

    ${TEST_HEADERS}
    ../../src/awards.h
    ../../src/adif.h
    ../../src/awarddxmarathon.h
    ../../src/callsign.h
    ../../src/database/queryexecutor.h
    ../../src/database/db_adif_primary_subdvisions_data.h
    ../../src/frequency.h
    ../../src/database.h
    ../../src/dataproxy_sqlite.h
    ../../src/filemanager.h
    ../../src/locator.h
    ../../src/logmodel.h
    ../../src/logwindow.h
    ../../src/qso.h
    ../../src/world.h
    ../../src/utilities.h
)

# Enable MOC for Qt signals/slots if needed
set(CMAKE_AUTOMOC ON)

# Link with Qt6 Core and Test
target_link_libraries(tst_logwindow
    Qt6::Core
    Qt6::Test
)

# Register the test with CTest
add_test(NAME tst_logwindow COMMAND tst_logwindow)

