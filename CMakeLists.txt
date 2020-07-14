cmake_minimum_required(VERSION 3.14)
project(riscv CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_EXPORT_COMPILE_COMMANDS, ON)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_LIST_DIR}/cmake")

include_directories(commons/include)
include_directories(uvm/sc/include)

find_package(SystemC)
if (NOT SYSTEMC_FOUND)
    include(ExternalProject)
    ExternalProject_Add(SystemC
            URL http://localhost:8080/systemc-2.3.3.tar.gz
            PREFIX ${CMAKE_SOURCE_DIR}/external/SystemC
            CMAKE_COMMAND "${CMAKE_COMMAND}"
            CMAKE_ARGS
                -DCMAKE_CXX_STANDARD:number=11
                -DCMAKE_INSTALL_PREFIX:string=<INSTALL_DIR>
            BUILD_COMMAND make -j$(nproc) && make install -j$(nproc)
            )
    ExternalProject_Add(UVM-SystemC
            URL http://localhost:8080/archive.tar
            PREFIX ${CMAKE_SOURCE_DIR}/external/SystemC
            CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env CXXFLAGS=-std=c++11 <SOURCE_DIR>/configure --enable-shared
                --prefix=<INSTALL_DIR>
                --with-systemc=${CMAKE_SOURCE_DIR}/external/SystemC/
                --with-arch-suffix=
            BUILD_COMMAND make -j$(nproc) && make install -j$(nproc)
            DEPENDS SystemC
            )


    include_directories(external/SystemC/include)
    link_directories(external/SystemC/lib)
endif ()

add_subdirectory(tlm)
add_subdirectory(uvm)
add_subdirectory(rtl)
include(CTest)
add_subdirectory(tests)
