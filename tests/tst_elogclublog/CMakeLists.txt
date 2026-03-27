cmake_minimum_required(VERSION 3.28)
project(tst_elogclublog)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network Test Widgets)
set(CMAKE_AUTOMOC ON)

file(GLOB TEST_SOURCES "*.cpp")
file(GLOB TEST_HEADERS "*.h")

add_executable(tst_elogclublog
    ${TEST_SOURCES}
    ../../src/elog/elogclublog.cpp
    ../../src/adif.cpp
    ../../src/callsign.cpp
    ../../src/locator.cpp
    ../../src/utilities.cpp

    ${TEST_HEADERS}
    ../../src/elog/elogclublog.h
    ../../src/adif.h
    ../../src/callsign.h
    ../../src/locator.h
    ../../src/utilities.h
    ../../src/klogdefinitions.h
)

target_link_libraries(tst_elogclublog
    Qt6::Core
    Qt6::Network
    Qt6::Test
    Qt6::Widgets
)

add_test(NAME tst_elogclublog COMMAND tst_elogclublog)
set_tests_properties(tst_elogclublog PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
