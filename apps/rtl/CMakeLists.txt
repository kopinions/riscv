set(bridge_lib_name "bridge")
set(BRIDGE_SRCS
        rtl.cpp)

set(CC gcc)
set(CXX g++)
add_library(${bridge_lib_name} SHARED)
target_link_libraries(${bridge_lib_name} PUBLIC remoteport-tlm)
target_sources(${bridge_lib_name} PRIVATE
        ${BRIDGE_SRCS})

target_link_libraries(${bridge_lib_name} PUBLIC systemc)

target_compile_options(${bridge_lib_name} PUBLIC
        $<$<CONFIG:DEBUG>:-g3>
        $<$<CONFIG:DEBUG>:-Og>
        $<$<CONFIG:RELEASE>:-O3>)
target_include_directories(${bridge_lib_name} PUBLIC ${CMAKE_SOURCE_DIR}/csrc/sysc/include)
target_include_directories(${bridge_lib_name} PUBLIC
        ${VCS_HOME}/etc/systemc/tlm/tli
        ${VCS_HOME}/include/systemc232
        ${VCS_HOME}/include
        ${VCS_HOME}/include/cosim/bf
        )
target_compile_definitions(${bridge_lib_name} PUBLIC SC_CPLUSPLUS=201703L)Ω
target_compile_definitions(${bridge_lib_name} PUBLIC
        TLI_BYTE_VIEW_DEBUG
        VCS
        )
target_compile_features(${bridge_lib_name} PUBLIC cxx_std_17)

add_custom_command(
        TARGET ${bridge_lib_name} PRE_LINK
        COMMAND syscan -full64 -cpp $(CXX) -cc $(CC) -kdb -lca -Mdir=csrc -cflags "$(CPPFLAGS) $(CXXFLAGS)" $(BRIDGE_SRCS)
)

add_custom_command(
        OUTPUT sysc_verilog_top.portmap
        COMMAND
        vlogan -full64 -cpp ${CXX} -cc ${CC} -kdb -lca -debug_acc+r+w+dmptf -debug_region+cell+encrypt -sverilog -sysc -sysc=opt_if -sysc=gen_portmap sysc_verilog_top.sv -sc_model sysc_verilog_top
        COMMAND
        vlogan -full64 -cpp ${CXX} -cc ${CC} -kdb -lca -debug_acc+r+w+dmptf -debug_region+cell+encrypt -sverilog -sysc -sysc=opt_if sysc_verilog_top.sv -sc_model sysc_verilog_top -sc_portmap sysc_verilog_top.portmap
        DEPENDS sysc_verilog_top.sv
)


add_custom_command(
        OUTPUT rtl.elf
        COMMAND
        vcs $(MAKE_PARALLEL) -sysc -full64 -cpp ${CXX} -cc ${CC} -kdb -lca libbridge.so -sysc=show_sc_main -top sc_main -ntb_opts uvm -debug_access+all -timescale=1ps/1fs -o rtl.elf
        DEPENDS ${bridge_lib_name}
)


#add_subdirectory(src)

