add_library(soc SHARED)

target_include_directories(soc PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)

target_include_directories(soc PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/libs/tlmsoc>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/libs/tlmsoc>)

target_include_directories(soc PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/libs/tlmsoc/libremote-port>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/libs/tlmsoc>)

target_sources(soc PRIVATE
        ${PROJECT_SOURCE_DIR}/libs/tlmsoc/libremote-port/remote-port-tlm-ats.cc
        ${PROJECT_SOURCE_DIR}/libs/tlmsoc/libremote-port/remote-port-tlm-memory-master.cc
        ${PROJECT_SOURCE_DIR}/libs/tlmsoc/libremote-port/remote-port-tlm-memory-slave.cc
        ${PROJECT_SOURCE_DIR}/libs/tlmsoc/libremote-port/remote-port-tlm-pci-ep.cc
        ${PROJECT_SOURCE_DIR}/libs/tlmsoc/libremote-port/remote-port-tlm-wires.cc
        ${PROJECT_SOURCE_DIR}/libs/tlmsoc/libremote-port/remote-port-tlm.cc
        ${PROJECT_SOURCE_DIR}/libs/tlmsoc/soc/pci/core/pci-device-base.cc
        )

add_library(libsystemctlm SHARED)
target_sources(libsystemctlm PRIVATE
        ${PROJECT_SOURCE_DIR}/libs/tlmsoc/libremote-port/remote-port-proto.c
        ${PROJECT_SOURCE_DIR}/libs/tlmsoc/libremote-port/remote-port-sk.c
        ${PROJECT_SOURCE_DIR}/libs/tlmsoc/libremote-port/safeio.c
        )
target_include_directories(libsystemctlm PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/libs/tlmsoc/libremote-port>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/libs/tlmsoc>)

target_link_libraries(soc PUBLIC systemc libsystemctlm)
target_compile_options(soc PRIVATE -std=c++17 -DSC_CPLUSPLUS=201703L)
add_subdirectory(src)

