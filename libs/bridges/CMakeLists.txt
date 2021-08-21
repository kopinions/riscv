add_library(bridges INTERFACE)

target_compile_features(bridges INTERFACE cxx_std_17)
target_include_directories(bridges INTERFACE include)
target_link_libraries(bridges INTERFACE systemc)
target_compile_options(bridges INTERFACE "$<$<CONFIG:DEBUG>:-g3>")
target_compile_options(bridges INTERFACE "$<$<CONFIG:RELEASE>:-O2>")
add_library(riscv::bridges ALIAS bridges)