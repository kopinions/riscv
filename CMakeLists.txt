cmake_minimum_required(VERSION 3.14)
project(riscv CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_EXPORT_COMPILE_COMMANDS, ON)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_LIST_DIR}/cmake")

include_directories(commons/include)
include_directories(bridges)
include_directories(tlm/include)
include_directories(uvm/sc/include)
include_directories(isa/include)

find_package(SystemC)
if (NOT SYSTEMC_FOUND)
    include(ExternalProject)
    ExternalProject_Add(SystemC
            URL https://www.accellera.org/images/downloads/standards/systemc/systemc-2.3.3.tar.gz
            PREFIX ${CMAKE_SOURCE_DIR}/external/SystemC
            CMAKE_COMMAND "${CMAKE_COMMAND}"
            CMAKE_ARGS
            -DCMAKE_CXX_STANDARD:number=14
            -DCMAKE_INSTALL_PREFIX:string=<INSTALL_DIR>
            BUILD_COMMAND make -j$(nproc) && make install -j$(nproc)
            )
    ExternalProject_Add(UVM-SystemC
            URL https://www.accellera.org/images/downloads/drafts-review/uvm-systemc-10-beta3tar.gz
            PREFIX ${CMAKE_SOURCE_DIR}/external/SystemC
            CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env CXXFLAGS=-std=c++14 <SOURCE_DIR>/configure --enable-shared
            --prefix=<INSTALL_DIR>
            --with-systemc=${CMAKE_SOURCE_DIR}/external/SystemC/
            --with-arch-suffix=
            BUILD_COMMAND make -j$(nproc) && make install -j$(nproc)
            DEPENDS SystemC
            )

    include_directories(external/SystemC/include)
    link_directories(external/SystemC/lib)
endif ()

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.10.0
)
FetchContent_GetProperties(googletest)
if (NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
    include_directories(${googletest_SOURCE_DIR}/googletest/include)
endif ()

include(GoogleTest)

enable_testing()

macro(gtest TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    target_link_libraries(${TESTNAME} PUBLIC gtest gmock gtest_main)

    gtest_discover_tests(${TESTNAME}
            TEST_PREFIX "${TESTNAME}."
            PROPERTIES FOLDER "tests")
endmacro()

add_subdirectory(tlm)
add_subdirectory(uvm)
add_subdirectory(rtl)
include(CTest)
add_subdirectory(tests)
