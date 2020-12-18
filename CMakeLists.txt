cmake_minimum_required(VERSION 3.14)
project(riscv CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_EXPORT_COMPILE_COMMANDS, ON)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}"
        "${CMAKE_CURRENT_LIST_DIR}/3rdparty/sysc"
        "${CMAKE_CURRENT_LIST_DIR}/3rdparty/uvm"
        "${CMAKE_CURRENT_LIST_DIR}/3rdparty/scv")

include_directories(commons/include)
include_directories(bridges)
include_directories(mcu/tlm/include)
include_directories(uvm/sc/include)
include_directories(isa/include)

find_package(SystemC
        COMPONENTS UVM SCV)
if (NOT SYSTEMC_FOUND)
    include(ExternalProject)
    ExternalProject_Add(SystemC
            URL https://www.accellera.org/images/downloads/standards/systemc/systemc-2.3.3.tar.gz
            INSTALL_DIR ${CMAKE_SOURCE_DIR}/3rdparty/sysc
            CMAKE_COMMAND "${CMAKE_COMMAND}"
            CMAKE_ARGS
            -DCMAKE_CXX_STANDARD:number=20
            -DCMAKE_INSTALL_PREFIX:string=<INSTALL_DIR>
            BUILD_COMMAND make -j$(nproc) && make install -j$(nproc)
            )
    ExternalProject_Add(UVM-SystemC
            URL https://www.accellera.org/images/downloads/drafts-review/uvm-systemc-10-beta3tar.gz
            INSTALL_DIR ${CMAKE_SOURCE_DIR}/3rdparty/uvm
            CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env CXXFLAGS=-std=c++2a <SOURCE_DIR>/configure --enable-shared
            --prefix=<INSTALL_DIR>
            --with-systemc=${CMAKE_SOURCE_DIR}/3rdparty/sysc
            --with-arch-suffix=
            BUILD_COMMAND make -j$(nproc) && make install -j$(nproc)
            DEPENDS SystemC
            )

    ExternalProject_Add(SCV
            URL https://www.accellera.org/images/downloads/standards/systemc/scv-2.0.1.tar.gz
            INSTALL_DIR ${CMAKE_SOURCE_DIR}/3rdparty/scv
            CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env CXXFLAGS=-std=c++2a <SOURCE_DIR>/configure --enable-shared
            --prefix=<INSTALL_DIR>
            --with-systemc=${CMAKE_SOURCE_DIR}/3rdparty/sysc
            --with-arch-suffix=
            BUILD_COMMAND make -j$(nproc) && make install -j$(nproc)
            DEPENDS SystemC
            )

    include_directories(3rdparty/uvm/include)
    link_directories(3rdparty/uvm/lib)

    include_directories(3rdparty/sysc/include)
    link_directories(3rdparty/sysc/lib)

    include_directories(3rdparty/scv/include)
    link_directories(3rdparty/scv/lib)
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

add_subdirectory(uvm)
add_subdirectory(mcu)
include(CTest)
add_subdirectory(tests)
