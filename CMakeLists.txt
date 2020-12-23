cmake_minimum_required(VERSION 3.14)
project(riscv CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_EXPORT_COMPILE_COMMANDS, ON)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}"
        "${CMAKE_CURRENT_LIST_DIR}/3rdparty/sysc"
        "${CMAKE_CURRENT_LIST_DIR}/3rdparty/uvm"
        "${CMAKE_CURRENT_LIST_DIR}/3rdparty/scv")

find_package(SystemC
        COMPONENTS UVM SCV)
if (NOT SYSTEMC_FOUND)
    include(ExternalProject)
    ExternalProject_Add(SystemC
            URL file:///Volumes/bank/workspace/disposables/archives/systemc-2.3.3.tar.gz
            INSTALL_DIR ${CMAKE_SOURCE_DIR}/3rdparty/sysc
            CMAKE_COMMAND "${CMAKE_COMMAND}"
            CMAKE_ARGS
            -DCMAKE_CXX_STANDARD:number=17
            -DSC_CPLUSPLUS:string=201703L
            -DCMAKE_INSTALL_PREFIX:string=<INSTALL_DIR>
            BUILD_COMMAND make -j$(nproc) && make install -j$(nproc)
            )

    ExternalProject_Add(UVM-SystemC
            URL file:///Volumes/bank/workspace/disposables/archives/archive.tar
            INSTALL_DIR ${CMAKE_SOURCE_DIR}/3rdparty/uvm
            CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env CXXFLAGS='-std=c++17 -DSC_CPLUSPLUS=201703L' <SOURCE_DIR>/configure --enable-shared
            --prefix=<INSTALL_DIR>
            --with-systemc=${CMAKE_SOURCE_DIR}/3rdparty/sysc
            --with-arch-suffix=
            BUILD_COMMAND make -j$(nproc) && make install -j$(nproc)
            DEPENDS SystemC
            )

    ExternalProject_Add(SCV
            URL file:///Volumes/bank/workspace/disposables/archives/scv-2.0.1.tar.gz
            INSTALL_DIR ${CMAKE_SOURCE_DIR}/3rdparty/scv
            CONFIGURE_COMMAND <SOURCE_DIR>/configure
            --prefix=<INSTALL_DIR>
            --with-pic
            --with-systemc=${CMAKE_SOURCE_DIR}/3rdparty/sysc CPPFLAGS=-DSC_CPLUSPLUS=201703L CXXFLAGS="-std=c++17 -DSC_CPLUSPLUS=201703L"
            BUILD_COMMAND make CPPFLAGS='-DSC_CPLUSPLUS=201703L' CXXFLAGS='-std=c++17' -j$(nproc) && make install -j$(nproc)
            DEPENDS SystemC
            )
    include_directories(${CMAKE_CURRENT_LIST_DIR}/3rdparty/uvm/include)
    link_directories(${CMAKE_CURRENT_LIST_DIR}/3rdparty/uvm/lib)

    include_directories(${CMAKE_CURRENT_LIST_DIR}/3rdparty/scv/include)
    link_directories(${CMAKE_CURRENT_LIST_DIR}/3rdparty/scv/lib)

    include_directories(${CMAKE_CURRENT_LIST_DIR}/3rdparty/sysc/include)
    link_directories(${CMAKE_CURRENT_LIST_DIR}/3rdparty/sysc/lib)
    link_libraries(systemc uvm-systemc scv)
else ()
    include_directories(${SYSTEMC_INCLUDE_DIRS})
    link_directories(${SYSTEMC_LIBRARY_DIRS})
    link_libraries(${SYSTEMC_LIBRARIES})
endif ()

include_directories(commons/include)
include_directories(bridges)
include_directories(mcu/tlm/include)
include_directories(uvm/sc/include)
include_directories(isa/include)
add_subdirectory(uvm)
add_subdirectory(mcu)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY /Volumes/bank/workspace/disposables/codes/googletest
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
    message(STATUS ${SYSTEMC_LIBRARY_DIRS})
    message(STATUS ${SYSTEMC_LIBRARIES})
    target_include_directories(${TESTNAME} PRIVATE ${SYSTEMC_INCLUDE_DIRS})
    target_link_directories(${TESTNAME} PUBLIC ${SYSTEMC_LIBRARY_DIRS})
    target_link_libraries(${TESTNAME} PUBLIC gtest gmock gtest_main ${SYSTEMC_LIBRARIES})
    target_compile_options(${TESTNAME} PRIVATE -std=c++17 -DSC_CPLUSPLUS=201703L)
    gtest_discover_tests(${TESTNAME}
            TEST_PREFIX "${TESTNAME}."
            PROPERTIES FOLDER "tests")
endmacro()
include(CTest)
add_subdirectory(tests)
