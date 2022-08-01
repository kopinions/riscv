add_executable(tlm.elf)

target_link_libraries(tlm.elf PUBLIC soc)
target_sources(tlm.elf PRIVATE
        tlm.cpp
        )


target_link_libraries(tlm.elf PUBLIC systemc)
target_compile_options(tlm.elf PRIVATE -std=c++17 -DSC_CPLUSPLUS=201703L -g3)
#add_subdirectory(src)

