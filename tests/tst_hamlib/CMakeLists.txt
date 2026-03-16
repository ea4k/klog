if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    cmake_minimum_required(VERSION 3.30)
    project(tst_hamlib)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Point CMake to your custom Find modules (e.g. FindHamlib.cmake) in the project root
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")

# 2. Find dependencies FIRST (before creating executables)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Test Widgets)

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
)

# 5. Find and link Hamlib
set(_hamlib_found FALSE)

# Prefer a config package if available (e.g., Homebrew/macOS or user-provided)
find_package(Hamlib CONFIG QUIET)
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

set(_tst_libs Qt6::Core Qt6::Gui Qt6::Test Qt6::Widgets)

if(TARGET Hamlib::Hamlib)
    list(APPEND _tst_libs Hamlib::Hamlib)
elseif(TARGET PkgConfig::HAMLIB)
    list(APPEND _tst_libs PkgConfig::HAMLIB)
elseif(HAMLIB_LIBRARIES)
    target_include_directories(tst_hamlib PRIVATE ${HAMLIB_INCLUDE_DIRS})
    list(APPEND _tst_libs ${HAMLIB_LIBRARIES})
else()
    message(FATAL_ERROR
        "Hamlib not found.\n"
        "Linux: sudo apt install libhamlib-dev (or equivalent)\n"
        "macOS: brew install hamlib (then set Hamlib_DIR or use FindHamlib.cmake)\n"
        "Windows: install Hamlib and set Hamlib_DIR to its CMake package or provide libs/headers."
    )
endif()

target_link_libraries(tst_hamlib PRIVATE ${_tst_libs})

# 6. Register the test
add_test(NAME tst_hamlib COMMAND tst_hamlib)
