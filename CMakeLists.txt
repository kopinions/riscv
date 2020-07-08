cmake_minimum_required(VERSION 3.1)
project(riscv CXX)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_LIST_DIR}/cmake")

add_subdirectory(tlm)
add_subdirectory(uvm)
add_subdirectory(rtl)
include(CTest)
add_subdirectory(tests)
