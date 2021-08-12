include(CTest)
include(FetchGoogleTest)
include(GoogleTest)

enable_testing()

macro(gtest TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    add_dependencies(${TESTNAME} UVM-SystemC)
    target_compile_options(${TESTNAME} PRIVATE -std=c++17)
    target_link_libraries(${TESTNAME} PUBLIC gtest gmock gtest_main)

    target_include_directories(${TESTNAME} PRIVATE ${SYSTEMC_INCLUDE_DIRS})
    target_link_libraries(${TESTNAME} PRIVATE ${SYSTEMC_LIBRARIES})
    gtest_discover_tests(${TESTNAME}
            TEST_PREFIX "${TESTNAME}."
            PROPERTIES FOLDER "tests")
endmacro()

add_subdirectory(commons)
add_subdirectory(isa)
add_subdirectory(tlm)
add_subdirectory(rtl)
