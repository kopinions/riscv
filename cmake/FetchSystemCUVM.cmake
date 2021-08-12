include(ExternalProject)

ExternalProject_Add(UVM-SystemC
        URL https://github.com/kopinions/uvm-systemc/archive/refs/heads/main.zip
        CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env CXXFLAGS=-std=c++17 <SOURCE_DIR>/configure --enable-shared
        --with-systemc=$<TARGET_FILE_DIR:systemc>
        --with-arch-suffix=
        BUILD_COMMAND make -j$(nproc)
        )

ExternalProject_Get_Property(UVM-SystemC BINARY_DIR)
message(FATAL ${BINARY_DIR})
add_library(uvm-systemc SHARED IMPORTED GLOBAL)
add_dependencies(uvm-systemc systemc)
add_dependencies(uvm-systemc UVM-SystemC)
set_target_properties(uvm-systemc PROPERTIES
        IMPORTED_LOCATION ${BINARY_DIR}
        INTERFACE_INCLUDE_DIRECTORIES ${BINARY_DIR})

#include(FetchContent)
#FetchContent_Declare(UVM-SystemC
#        URL https://github.com/kopinions/uvm-systemc/archive/refs/heads/main.zip)
#FetchContent_GetProperties(UVM-SystemC)
#if (NOT uvm-systemc_POPULATED)
#    FetchContent_Populate(UVM-SystemC)
#    message(STATUS ${uvm-systemc_SOURCE_DIR})
#endif ()