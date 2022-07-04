include(ExternalProject)
include(GNUInstallDirs)
ExternalProject_Add(uvm-systemc-build
        URL https://github.com/kopinions/uvm-systemc/archive/refs/heads/main.zip
        CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env CXXFLAGS=-std=c++17 SYSTEMC_CFLAGS=-std=c++17 LD_LIBRARY_PATH=$<TARGET_PROPERTY:systemc,PREFIX>/lib64:${LD_LIBRARY_PATH} <SOURCE_DIR>/configure --enable-shared
        --with-systemc=$<TARGET_PROPERTY:systemc,PREFIX> --prefix=<INSTALL_DIR>
        --with-arch-suffix=64 && make -j$(nproc) && make -j$(nproc) install
        )
ExternalProject_Get_Property(uvm-systemc-build INSTALL_DIR)
set(UVM_SYSTEMC_ROOT ${INSTALL_DIR})
set(UVM_SYSTEMC_LIBDIR ${UVM_SYSTEMC_ROOT}/${CMAKE_INSTALL_LIBDIR})
set(UVM_SYSTEMC_LIBFILE ${UVM_SYSTEMC_LIBDIR}/libuvm-systemc${CMAKE_SHARED_LIBRARY_SUFFIX})
set(UVM_SYSTEMC_INCLUDE ${UVM_SYSTEMC_ROOT}/${CMAKE_INSTALL_INCLUDEDIR})
file(MAKE_DIRECTORY ${UVM_SYSTEMC_LIBDIR})
file(TOUCH ${UVM_SYSTEMC_LIBFILE})
file(MAKE_DIRECTORY ${UVM_SYSTEMC_INCLUDE})

add_library(uvm-systemc SHARED IMPORTED GLOBAL)
add_dependencies(uvm-systemc-build systemc)
add_dependencies(uvm-systemc uvm-systemc-build)
set_target_properties(uvm-systemc PROPERTIES
        IMPORTED_LOCATION ${UVM_SYSTEMC_LIBFILE}
        INTERFACE_INCLUDE_DIRECTORIES ${UVM_SYSTEMC_INCLUDE})
