add_library(recording SHARED)

target_include_directories(recording PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)

target_link_libraries(recording PUBLIC systemc scv)
target_compile_options(recording PRIVATE -std=c++17 -DSC_CPLUSPLUS=201703L)
add_subdirectory(src)

