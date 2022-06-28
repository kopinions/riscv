add_library(cosim SHARED)

target_include_directories(cosim PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)

target_include_directories(cosim PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/libs/tlmsoc/libremote-port>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/libs/tlmsoc/libremote-port>)

target_link_libraries(cosim PUBLIC systemc)
target_compile_options(cosim PRIVATE -std=c++17 -DSC_CPLUSPLUS=201703L)
add_subdirectory(src)

