set(hdl_name cpu)

add_executable(${hdl_name}_test
  fetch_test.cpp
  main.cpp
        )

target_compile_options(${hdl_name}_test
        PRIVATE "-std=c++11"
        )

target_include_directories(${hdl_name}_test SYSTEM PRIVATE
        ${COMMONS_INCLUDE_DIR}
        ${UVM_INCLUDE_DIR}
        ${SYSTEMC_INCLUDE_DIRS}
        )

set_target_properties(${hdl_name}_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY
        "${CMAKE_BINARY_DIR}/sc/uts/${hdl_name}"
        )

target_link_libraries(${hdl_name}_test
        PUBLIC uvm-systemc systemc)

set(TESTS
        fetch_test
        )

foreach (test ${TESTS})
    add_test(
            NAME
            ${hdl_name}_${test}
            COMMAND
            ${hdl_name}_test
            +UVM_TESTNAME=${test}
            +uvm_set_config_string=*,trace_filename,${hdl_name}_${test}
            WORKING_DIRECTORY
            "${CMAKE_BINARY_DIR}/sc/uts/${hdl_name}"
    )
endforeach ()
