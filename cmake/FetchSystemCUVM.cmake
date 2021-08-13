include(ExternalProject)
message(STATUS  xxx)
file(GENERATE OUTPUT xx CONTENT $<TARGET_PROPERTY:systemc,BINARY_DIR>)
ExternalProject_Add(UVM-SystemC
        URL https://github.com/kopinions/uvm-systemc/archive/refs/heads/main.zip
        CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env CXXFLAGS=-std=c++17 <SOURCE_DIR>/configure --enable-shared
        --with-systemc=/Volumes/data/workspace/1si/ic/riscv/build/_deps/systemc-src/src
        --with-arch-suffix=
        BUILD_COMMAND make -j$(nproc)
        )

ExternalProject_Get_Property(UVM-SystemC BINARY_DIR)
message(FATAL ${BINARY_DIR})
add_library(uvm-systemc SHARED IMPORTED GLOBAL)
add_dependencies(UVM-SystemC systemc)
add_dependencies(uvm-systemc UVM-SystemC)
set_target_properties(uvm-systemc PROPERTIES
        IMPORTED_LOCATION ${BINARY_DIR}
        INTERFACE_INCLUDE_DIRECTORIES ${BINARY_DIR})