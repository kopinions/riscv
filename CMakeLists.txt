cmake_minimum_required(VERSION 3.14)
project(riscv CXX)
set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_LIST_DIR}/cmake")

add_subdirectory(libs/recording)
add_subdirectory(libs/bridges)
add_subdirectory(cosim)
message(STATUS ${PROJECT_SOURCE_DIR})
include_directories(uvm/sc/include)
include_directories(include)

find_package(SystemC
        COMPONENTS UVM)
if (NOT SYSTEMC_FOUND)
    include(FetchSystemC)
    include(FetchSystemCUVM)
    include(FetchSCV)
endif ()

add_subdirectory(uvm)
add_subdirectory(tests)
add_subdirectory(apps/tlm)
