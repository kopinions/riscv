include(CTest)
include(FetchGoogleTest)
include(GoogleTest)

enable_testing()

macro(gtest TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    target_compile_options(${TESTNAME} PRIVATE -std=c++17)
    target_link_libraries(${TESTNAME} PUBLIC gtest gmock gtest_main)
    target_link_libraries(${TESTNAME} PUBLIC systemc uvm-systemc recording bridges)
    gtest_discover_tests(${TESTNAME}
            TEST_PREFIX "${TESTNAME}."
            PROPERTIES FOLDER "tests")
endmacro()

add_subdirectory(commons)
add_subdirectory(isa)
add_subdirectory(tlm)
add_subdirectory(rtl)
