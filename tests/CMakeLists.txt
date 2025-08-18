cmake_minimum_required(VERSION 3.10)
project(KLogTests)

enable_testing()

# Add each test subdirectory (tst_* folders)
file(GLOB TEST_SUBDIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "tst_*")
foreach(subdir ${TEST_SUBDIRS})
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}")
        add_subdirectory(${subdir})
    endif()
endforeach()
