add_executable(tlm.elf)

target_link_libraries(tlm.elf PUBLIC remoteport-tlm)
target_sources(tlm.elf PRIVATE
        tlm.cpp
        )

target_link_libraries(tlm.elf PUBLIC systemc)

target_compile_options(tlm.elf PUBLIC
        $<$<CONFIG:DEBUG>:-g3>
        $<$<CONFIG:DEBUG>:-Og>
        $<$<CONFIG:RELEASE>:-O3>)
target_compile_definitions(tlm.elf PUBLIC SC_CPLUSPLUS=201703L)
target_compile_features(tlm.elf PUBLIC cxx_std_17)
#add_subdirectory(src)

