include(ExternalProject)
ExternalProject_Add(systemc-build
        URL https://www.accellera.org/images/downloads/standards/systemc/systemc-2.3.3.tar.gz
        CONFIGURE_COMMAND ${CMAKE_COMMAND} -DCMAKE_CXX_STANDARD:number=17
        -DCMAKE_INSTALL_PREFIX:string=<INSTALL_DIR> <SOURCE_DIR> && make -j$(nproc) && make install -j$(nproc)
        )
ExternalProject_Get_Property(systemc-build INSTALL_DIR)
set(SYSTEMC_ROOT ${INSTALL_DIR})
set(SYSTEMC_LIBDIR ${SYSTEMC_ROOT}/lib)
set(SYSTEMC_LIBFILE ${SYSTEMC_LIBDIR}/libsystemc.dylib)
set(SYSTEMC_INCLUDE ${SYSTEMC_ROOT}/include)
file(MAKE_DIRECTORY ${SYSTEMC_LIBDIR})
file(TOUCH ${SYSTEMC_LIBFILE})
file(MAKE_DIRECTORY ${SYSTEMC_INCLUDE})

add_library(systemc SHARED IMPORTED GLOBAL)
add_dependencies(systemc systemc-build)
set_target_properties(systemc PROPERTIES
        IMPORTED_LOCATION ${SYSTEMC_LIBFILE}
        PREFIX ${SYSTEMC_ROOT}
        INTERFACE_INCLUDE_DIRECTORIES ${SYSTEMC_INCLUDE})