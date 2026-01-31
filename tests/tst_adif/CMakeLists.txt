cmake_minimum_required(VERSION 3.30)
project(tst_adif)

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
add_executable(tst_adif
    ${TEST_SOURCES}
    ../../src/callsign.cpp
    ../../src/adif.cpp

    #${TEST_HEADERS}
    ../../src/adif.h
    ../../src/callsign.h
    ../../src/klogdefinitions.h
)

# Link with Qt6 Core and Test
target_link_libraries(tst_adif
    Qt6::Core
    Qt6::Test
)

# Register the test with CTest
add_test(NAME tst_adif COMMAND tst_adif)

