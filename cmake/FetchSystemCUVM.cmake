include(ExternalProject)
ExternalProject_Add(uvm-systemc-build
        URL file:///Users/neo/Desktop/main.zip
        CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env CXXFLAGS=-std=c++17 <SOURCE_DIR>/configure --enable-shared
        --with-systemc=$<TARGET_PROPERTY:systemc,PREFIX> --prefix=<INSTALL_DIR>
        --with-arch-suffix= && make -j$(nproc) && make -j$(nproc) install
        )
ExternalProject_Get_Property(uvm-systemc-build INSTALL_DIR)
set(UVM_SYSTEMC_ROOT ${INSTALL_DIR})
set(UVM_SYSTEMC_LIBFILE ${UVM_SYSTEMC_ROOT}/lib/libuvm-systemc.dylib)
set(UVM_SYSTEMC_INCLUDE ${UVM_SYSTEMC_ROOT}/include)
file(TOUCH ${UVM_SYSTEMC_LIBFILE})
file(MAKE_DIRECTORY ${UVM_SYSTEMC_INCLUDE})

add_library(uvm-systemc SHARED IMPORTED GLOBAL)
add_dependencies(uvm-systemc-build systemc)
add_dependencies(uvm-systemc uvm-systemc-build)
set_target_properties(uvm-systemc PROPERTIES
        IMPORTED_LOCATION ${UVM_SYSTEMC_LIBFILE}
        INTERFACE_INCLUDE_DIRECTORIES ${UVM_SYSTEMC_INCLUDE})