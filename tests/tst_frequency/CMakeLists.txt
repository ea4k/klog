cmake_minimum_required(VERSION 3.16)
project(tst_frequency)

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
add_executable(tst_frequency
    ${TEST_SOURCES}
    ../../src/frequency.cpp

    ${TEST_HEADERS}
    ../../src/frequency.h
    ../../src/klogdefinitions.h
)

# Enable MOC for Qt signals/slots if needed
set(CMAKE_AUTOMOC ON)

# Link with Qt6 Core and Test
target_link_libraries(tst_frequency
    Qt6::Core
    Qt6::Test
)

# Register the test with CTest
add_test(NAME tst_frequency COMMAND tst_frequency)

