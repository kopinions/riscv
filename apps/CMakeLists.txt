add_executable(cosim SHARED)

target_link_libraries(cosim PUBILC soc)
target_sources(cosim PRIVATE
        cosim.cpp
        )


target_link_libraries(cosim PUBLIC systemc)
target_compile_options(cosim PRIVATE -std=c++17 -DSC_CPLUSPLUS=201703L)
add_subdirectory(src)

