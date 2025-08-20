cmake_minimum_required(VERSION 3.30)
project(tst_dxspot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 Core and Test modules
find_package(Qt6 REQUIRED COMPONENTS Core Test)
# Enable AUTOMOC before add_executable
set(CMAKE_AUTOMOC ON)

# Collect test sources
file(GLOB TEST_SOURCES "*.cpp")
# Exclude any stale/generated files that might be in the source tree
list(FILTER TEST_SOURCES EXCLUDE REGEX "/moc_.*\\.cpp$")
list(FILTER TEST_SOURCES EXCLUDE REGEX "/ui_.*\\.cpp$")
list(FILTER TEST_SOURCES EXCLUDE REGEX "/qrc_.*\\.cpp$")

file(GLOB TEST_HEADERS "*.h")

# Create the test executable
add_executable(tst_dxspot
    ${TEST_SOURCES}
    ../../src/frequency.cpp
    ../../src/callsign.cpp
    ../../src/dxcluster/dxspot.cpp

    ${TEST_HEADERS}
    ../../src/frequency.h
    ../../src/callsign.h
    ../../src/dxcluster/dxspot.h
    ../../src/klogdefinitions.h
)

# Link with Qt6 Core and Test
target_link_libraries(tst_dxspot
    Qt6::Core
    Qt6::Test
)

# Register the test with CTest
add_test(NAME tst_dxspot COMMAND tst_dxspot)

