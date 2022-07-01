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


target_link_libraries(soc PUBLIC systemc)
target_compile_options(soc PRIVATE -std=c++17 -DSC_CPLUSPLUS=201703L)
add_subdirectory(src)

