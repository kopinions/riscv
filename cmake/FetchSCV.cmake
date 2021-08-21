include(ExternalProject)
ExternalProject_Add(scv-build
        URL https://www.accellera.org/images/downloads/standards/systemc/scv-2.0.1.tar.gz
        CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env CXXFLAGS=-std=c++17\ -DSC_CPLUSPLUS=201703L
        <SOURCE_DIR>/configure --enable-shared --with-pic --with-systemc=$<TARGET_PROPERTY:systemc,PREFIX> --prefix=<INSTALL_DIR>
        && ${CMAKE_COMMAND} -E env CXXFLAGS="-std=c++17 -DSC_CPLUSPLUS=201703L" CPPFLAGS="-DSC_CPLUSPLUS=201703L" env
        && make CXXFLAGS="-std=c++17" CPPFLAGS="-DSC_CPLUSPLUS=201703L" -j$(nproc)
        && make -j$(nproc) install
        )
ExternalProject_Get_Property(scv-build INSTALL_DIR)
set(SCV_ROOT ${INSTALL_DIR})
set(SCV_LIBDIR ${SCV_ROOT}/lib-macosx64)
set(SCV_LIBFILE ${SCV_LIBDIR}/libscv.dylib)
set(SCV_INCLUDE ${SCV_ROOT}/include)
file(MAKE_DIRECTORY ${SCV_LIBDIR})
file(TOUCH ${SCV_LIBFILE})
file(MAKE_DIRECTORY ${SCV_INCLUDE})

add_library(scv SHARED IMPORTED GLOBAL)
add_dependencies(scv scv-build)
set_target_properties(scv PROPERTIES
        IMPORTED_LOCATION ${SCV_LIBFILE}
        PREFIX ${SCV_ROOT}
        INTERFACE_INCLUDE_DIRECTORIES ${SCV_INCLUDE})