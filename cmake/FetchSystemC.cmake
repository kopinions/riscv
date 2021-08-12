include(ExternalProject)
ExternalProject_Add(SystemC
        URL https://www.accellera.org/images/downloads/standards/systemc/systemc-2.3.3.tar.gz
        INSTALL_DIR ${CMAKE_SOURCE_DIR}/3rdparty/sysc
        CMAKE_COMMAND "${CMAKE_COMMAND}"
        CMAKE_ARGS
        -DCMAKE_CXX_STANDARD:number=17
        -DCMAKE_INSTALL_PREFIX:string=<INSTALL_DIR>
        BUILD_COMMAND make -j$(nproc) && make install -j$(nproc)
        )

add_library(systemc SHARED IMPORTED GLOBAL)
add_dependencies(systemc SystemC)
set_target_properties(systemc PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/3rdparty/sysc/lib/libsystemc.dylib
        INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/3rdparty/sysc/include)
