cmake_minimum_required(VERSION 3.30)
project(tst_callsign)

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
add_executable(tst_callsign
    ${TEST_SOURCES}
    ../../src/callsign.cpp

    ${TEST_HEADERS}
    ../../src/callsign.h
    ../../src/klogdefinitions.h
)

# Link with Qt6 Core and Test
target_link_libraries(tst_callsign
    Qt6::Core
    Qt6::Test
)

# Register the test with CTest
add_test(NAME tst_callsign COMMAND tst_callsign)

