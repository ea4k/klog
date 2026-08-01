if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    cmake_minimum_required(VERSION 3.30)
    project(tst_clublogmostwanted)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 Core, Network and Test modules
find_package(Qt6 REQUIRED COMPONENTS Core Network Test)
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
add_executable(tst_clublogmostwanted
    ${TEST_SOURCES}
    ../../src/dxcluster/clublogmostwanted.cpp

    ${TEST_HEADERS}
    ../../src/dxcluster/clublogmostwanted.h
)

# Link with Qt6 Core, Network and Test
target_link_libraries(tst_clublogmostwanted
    Qt6::Core
    Qt6::Network
    Qt6::Test
)

# Register the test with CTest
add_test(NAME tst_clublogmostwanted COMMAND tst_clublogmostwanted)
set_tests_properties(tst_clublogmostwanted PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
