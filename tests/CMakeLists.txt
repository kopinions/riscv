add_subdirectory(commons)
add_subdirectory(isa)
add_subdirectory(tlm)
add_subdirectory(rtl)

add_executable(xxx
        sc_version.cpp
        )
target_include_directories(xxx SYSTEM PRIVATE
        /Volumes/data/workspace/1si/ic/riscv/3rdparty/sysc/include
        )

target_link_libraries(xxx
        PUBLIC systemc)