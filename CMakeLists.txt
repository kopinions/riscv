cmake_minimum_required(VERSION 3.14)
project(riscv CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS, ON)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_LIST_DIR}/cmake")

include_directories(commons/include)
include_directories(uvm/sc/include)

find_package(SystemC COMPONENTS UVM)
if (NOT SYSTEMC_FOUND)
    include(ExternalProject)
    ExternalProject_Add(SystemC
            URL https://www.accellera.org/images/downloads/standards/systemc/systemc-2.3.3.tar.gz
            CMAKE_COMMAND "${CMAKE_COMMAND} -DCMAKE_CXX_STANDARD=11"
            )
    ExternalProject_Add(UVM-SystemC
            URL https://www.accellera.org/images/downloads/drafts-review/uvm-systemc-10-beta3tar.gz
            CONFIGURE_COMMAND "./configure"
            MAKE_COMMAND ${MAKE_EXE}
            )
endif ()

add_subdirectory(tlm)
add_subdirectory(uvm)
add_subdirectory(rtl)
include(CTest)
add_subdirectory(tests)
