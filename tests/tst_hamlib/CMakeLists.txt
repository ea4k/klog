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
# Prefer a config package if available (e.g., Homebrew/macOS or user-provided)
find_package (Hamlib REQUIRED)
#find_package(Hamlib CONFIG QUIET)
if(Hamlib_FOUND)
    set(_hamlib_found TRUE)
endif()

# Try module mode (uses cmake/FindHamlib.cmake if present)
if(NOT _hamlib_found)
    find_package(Hamlib MODULE QUIET)
    if(Hamlib_FOUND)
        set(_hamlib_found TRUE)
    endif()
endif()

# Linux-only fallback to pkg-config
if(NOT _hamlib_found AND UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(HAMLIB REQUIRED IMPORTED_TARGET hamlib)
    set(_hamlib_found TRUE)
endif()

# Link to whichever method was found
if(TARGET Hamlib::Hamlib)
    target_link_libraries(klog PRIVATE Hamlib::Hamlib)
elseif(TARGET PkgConfig::HAMLIB)
    target_link_libraries(klog PRIVATE PkgConfig::HAMLIB)
elseif(HAMLIB_LIBRARIES)
    target_include_directories(klog PRIVATE ${HAMLIB_INCLUDE_DIRS})
    target_link_libraries(klog PRIVATE ${HAMLIB_LIBRARIES})
else()
    message(FATAL_ERROR
        "Hamlib not found.\n"
        "Linux: sudo apt install libhamlib-dev (or equivalent)\n"
        "macOS: brew install hamlib (then set Hamlib_DIR or use FindHamlib.cmake)\n"
        "Windows: install Hamlib and set Hamlib_DIR to its CMake package or provide libs/headers."
    )
endif()

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
