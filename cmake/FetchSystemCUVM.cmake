ExternalProject_Add(UVM-SystemC
        URL https://github.com/kopinions/uvm-systemc/archive/refs/heads/main.zip
        INSTALL_DIR ${CMAKE_SOURCE_DIR}/3rdparty/uvm
        CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env CXXFLAGS=-std=c++17 <SOURCE_DIR>/configure --enable-shared
        --prefix=<INSTALL_DIR>
        --with-systemc=${CMAKE_SOURCE_DIR}/3rdparty/sysc
        --with-arch-suffix=
        BUILD_COMMAND make -j$(nproc) && make install -j$(nproc)
        DEPENDS SystemC
        )

add_library(uvm-systemc SHARED IMPORTED GLOBAL)
add_dependencies(uvm-systemc UVM-SystemC)
set_target_properties(uvm-systemc PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/3rdparty/uvm/lib/libuvm-systemc.dylib
        INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/3rdparty/uvm/include)
