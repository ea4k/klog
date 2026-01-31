cmake_minimum_required(VERSION 3.30)
project(tst_hamlib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Point CMake to your custom Find modules (e.g. FindHamlib.cmake) in the project root
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")

# 2. Find dependencies FIRST (before creating executables)
find_package(Qt6 REQUIRED COMPONENTS Core Test Widgets)
find_package(Hamlib REQUIRED)

enable_testing()
set(CMAKE_AUTOMOC ON)

# Collect test sources
file(GLOB TEST_SOURCES "*.cpp")
file(GLOB TEST_HEADERS "*.h")

# 3. Create the test executable
add_executable(tst_hamlib
    ${TEST_SOURCES}
    ${TEST_HEADERS}
    # Include necessary KLog source files
    ../../src/callsign.cpp
    ../../src/frequency.cpp
    ../../src/locator.cpp
    ../../src/utilities.cpp
    ../../src/hamlibclass.cpp

    # Include headers for MOC and reference
    ../../src/klogdefinitions.h
    ../../src/callsign.h
    ../../src/frequency.h
    ../../src/locator.h
    ../../src/utilities.h
    ../../src/hamlibclass.h
)

# 4. Set Include Directories (Crucial for finding "hamlib/rig.h" and KLog headers)
target_include_directories(tst_hamlib PRIVATE
    ../../src
    ${Hamlib_INCLUDE_DIRS}
)

# 5. Link Libraries
target_link_libraries(tst_hamlib PRIVATE
    Qt6::Core
    Qt6::Test
    Qt6::Widgets
    ${Hamlib_LIBRARIES} # Link the Hamlib binary found by find_package
)

# 6. Register the test
add_test(NAME tst_hamlib COMMAND tst_hamlib)
