add_library(remoteport SHARED)
target_sources(remoteport PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/libremote-port/remote-port-proto.c
        ${CMAKE_CURRENT_LIST_DIR}/src/libremote-port/remote-port-sk.c
        ${CMAKE_CURRENT_LIST_DIR}/src/libremote-port/safeio.c
        )

target_include_directories(remoteport PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src/libremote-port>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/libs/tlmsoc>)

target_compile_options(remoteport PUBLIC
        $<$<CONFIG:DEBUG>:-g3>
        $<$<CONFIG:DEBUG>:-Og>
        $<$<CONFIG:RELEASE>:-O3>)

target_compile_options(remoteport PUBLIC
        -fPIC -m64)

add_library(remoteport-tlm SHARED)

target_include_directories(remoteport-tlm PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src/>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/tlmsoc>)

target_sources(remoteport-tlm PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/libremote-port/remote-port-tlm-ats.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/libremote-port/remote-port-tlm-memory-master.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/libremote-port/remote-port-tlm-memory-slave.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/libremote-port/remote-port-tlm-pci-ep.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/libremote-port/remote-port-tlm-wires.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/libremote-port/remote-port-tlm.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/soc/pci/core/pci-device-base.cc
        )

target_compile_options(remoteport-tlm PUBLIC
        $<$<CONFIG:DEBUG>:-g3>
        $<$<CONFIG:DEBUG>:-Og>
        $<$<CONFIG:RELEASE>:-O3>)

target_compile_options(remoteport-tlm PUBLIC
        -fPIC -m64)

target_link_libraries(remoteport-tlm PUBLIC systemc remoteport)
target_compile_definitions(remoteport-tlm PUBLIC SC_CPLUSPLUS=201703L)
target_compile_features(remoteport-tlm PUBLIC cxx_std_17)

