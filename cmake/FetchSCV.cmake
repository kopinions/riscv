include(GNUInstallDirs)
include(ExternalProject)
ExternalProject_Add(scv-build
        GIT_REPOSITORY https://github.com/kopinions/scv.git
        CONFIGURE_COMMAND pushd <SOURCE_DIR> && autoconf && popd &&
        ${CMAKE_COMMAND} -E env CXXFLAGS=-std=c++17\ -DSC_CPLUSPLUS=201703L
        <SOURCE_DIR>/configure --enable-shared --with-pic --with-systemc=$<TARGET_PROPERTY:systemc,PREFIX> --prefix=<INSTALL_DIR>
        && ${CMAKE_COMMAND} -E env CXXFLAGS="-std=c++17 -DSC_CPLUSPLUS=201703L" CPPFLAGS="-DSC_CPLUSPLUS=201703L"
        make CXXFLAGS="-std=c++17" CPPFLAGS="-DSC_CPLUSPLUS=201703L" -j$(nproc)
        && make -j$(nproc) install
        )
ExternalProject_Get_Property(scv-build INSTALL_DIR)
set(SCV_ROOT ${INSTALL_DIR})
set(SCV_LIBDIR ${SCV_ROOT}/${CMAKE_INSTALL_LIBDIR})
set(SCV_LIBFILE ${SCV_LIBDIR}/libscv${CMAKE_SHARED_LIBRARY_SUFFIX} )
set(SCV_INCLUDE ${SCV_ROOT}/${CMAKE_INSTALL_INCLUDEDIR})
file(MAKE_DIRECTORY ${SCV_LIBDIR})
file(TOUCH ${SCV_LIBFILE})
file(MAKE_DIRECTORY ${SCV_INCLUDE})

add_library(scv SHARED IMPORTED GLOBAL)
add_dependencies(scv scv-build)
add_dependencies(scv-build systemc)
set_target_properties(scv PROPERTIES
        IMPORTED_LOCATION ${SCV_LIBFILE}
        PREFIX ${SCV_ROOT}
        INTERFACE_INCLUDE_DIRECTORIES ${SCV_INCLUDE})
