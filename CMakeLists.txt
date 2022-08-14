cmake_minimum_required(VERSION 3.12)
project(riscv CXX C)
set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_LIST_DIR}/cmake")

add_subdirectory(libs/recording)
add_subdirectory(libs/bridges)
add_subdirectory(libs/tlmsoc)
include_directories(uvm/sc/include)
include_directories(include)

find_package(SystemC
        COMPONENTS UVM)
if (NOT SYSTEMC_FOUND)
    include(FetchSystemC)
endif ()

if (NOT UVM_SYSTEMC_FOUND)
    include(FetchSystemCUVM)
endif ()
if (NOT SCV_FOUND)
    include(FetchSCV)
endif ()

add_subdirectory(uvm)
add_subdirectory(tests)
add_subdirectory(apps/tlm)
