cmake_minimum_required(VERSION 3.16)
project(tst_wizard)

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
add_executable(tst_wizard
    ${TEST_SOURCES}
    ../../src/adif.cpp
    ../../src/callsign.cpp
    ../../src/downloadcty.cpp
    ../../src/locator.cpp
    ../../src/utilities.cpp
    ../../src/startwizard.cpp

    ${TEST_HEADERS}
    ../../src/adif.h
    ../../src/callsign.h
    ../../src/downloadcty.h
    ../../src/locator.h
    ../../src/utilities.h
    ../../src/startwizard.h
)

# Link with Qt6 Core and Test
target_link_libraries(tst_wizard
    Qt6::Core
    Qt6::Network
    Qt6::Test
    Qt6::Widgets
)

# Register the test with CTest
add_test(NAME tst_wizard COMMAND tst_wizard)

