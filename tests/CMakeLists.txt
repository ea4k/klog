enable_testing()

# Collect all test executable names and add their subdirectories
file(GLOB TEST_SUBDIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "tst_*")
set(ALL_TEST_TARGETS "")
foreach(subdir ${TEST_SUBDIRS})
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}")
        add_subdirectory(${subdir})
        list(APPEND ALL_TEST_TARGETS ${subdir})
    endif()
endforeach()

# Custom target to build all tests at once: cmake --build build --target build_tests
add_custom_target(build_tests DEPENDS ${ALL_TEST_TARGETS})

# NOTE: QT_QPA_PLATFORM=offscreen is set via set_tests_properties() in each
# test subdirectory's CMakeLists.txt (right after add_test).  CMake's
# set_tests_properties() only works on tests defined in the *same* directory
# scope, so it must live in the subdirectory, not here.
