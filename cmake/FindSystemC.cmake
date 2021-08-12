if (COMMAND _find_systemc)
    return()
endif ()

function(_find_systemc)
    find_package(PackageHandleStandardArgs REQUIRED)

    # SystemC

    find_library(SYSTEMC_LIBRARY systemc
            HINTS $ENV{SYSTEMC_LIBDIR} $ENV{SYSTEMC_HOME}
            PATH_SUFFIXES lib lib32 lib64 lib-linux lib-linux32 lib-linux64
            lib-cygwin lib-cygwin32 lib-cygwin64
            DOC "Path to the SystemC library"
            )

    if (SYSTEMC_LIBRARY)
        add_library(systemc UNKNOWN IMPORTED)
        set_target_properties(systemc PROPERTIES IMPORTED_LOCATION
                ${SYSTEMC_LIBRARY})
    endif ()
    mark_as_advanced(SYSTEMC_LIBRARY)

    if ("${CMAKE_VERSION}" VERSION_GREATER 2.8.12)
        get_filename_component(SYSTEMC_LIBRARY_DIR ${SYSTEMC_LIBRARY}
                DIRECTORY)
    endif ()
    mark_as_advanced(SYSTEMC_LIBRARY_DIR)

    find_path(SYSTEMC_INCLUDE_DIR systemc.h
            HINTS $ENV{SYSTEMC_INCLUDE} $ENV{SYSTEMC_HOME}/include
            DOC "Path to the SystemC headers"
            )

    mark_as_advanced(SYSTEMC_INCLUDE_DIR)

    find_package_handle_standard_args(SystemC REQUIRED_VARS
            SYSTEMC_LIBRARY_DIR SYSTEMC_LIBRARY SYSTEMC_INCLUDE_DIR)

    macro(find_systemc_component_scv)
        find_library(SCV_LIBRARY scv
                HINTS $ENV{SCV_LIBDIR} $ENV{SYSTEMC_LIBDIR} $ENV{SYSTEMC_HOME}
                PATH_SUFFIXES lib lib32 lib64 lib-linux lib-linux32 lib-linux64
                lib-cygwin lib-cygwin32 lib-cygwin64 lib-macosx64
                DOC "Path to the SystemC verification library"
                )

        if (SCV_LIBRARY)
            add_library(scv UNKNOWN IMPORTED)
            set_target_properties(scv PROPERTIES IMPORTED_LOCATION ${SCV_LIBRARY})
        endif ()

        mark_as_advanced(SCV_LIBRARY)

        if ("${CMAKE_VERSION}" VERSION_GREATER 2.8.12)
            get_filename_component(SCV_LIBRARY_DIR ${SCV_LIBRARY}
                    DIRECTORY)
        endif ()
        mark_as_advanced(SCV_LIBRARY_DIR)

        find_path(SCV_INCLUDE_DIR scv.h
                HINTS $ENV{SCV_INCLUDE} $ENV{SYSTEMC_INCLUDE} $ENV{SYSTEMC_HOME}/include
                DOC "Path to the SystemC verification headers"
                )

        mark_as_advanced(SCV_INCLUDE_DIR)

        find_package_handle_standard_args(SCV REQUIRED_VARS
                SCV_LIBRARY_DIR SCV_LIBRARY SCV_INCLUDE_DIR)

        if (SCV_FOUND)
            set(SYSTEMC_LIBRARY_DIRS ${SYSTEMC_LIBRARY_DIR} ${SYSTEMC_LIBRARY_DIRS} ${SCV_LIBRARY_DIR})
            set(SYSTEMC_LIBRARIES ${SYSTEMC_LIBRARIES} ${SYSTEMC_LIBRARY} scv)
            set(SYSTEMC_INCLUDE_DIRS ${SYSTEMC_INCLUDE_DIRS} ${SYSTEMC_INCLUDE_DIR} ${SCV_INCLUDE_DIR})
        else ()
            set(SYSTEMC_FOUND FALSE)
        endif ()
    endmacro()

    macro(find_systemc_component_uvm)
        find_library(UVM_SYSTEMC_LIBRARY uvm-systemc
                HINTS $ENV{UVM_SYSTEMC_LIBDIR} $ENV{SYSTEMC_LIBDIR} $ENV{SYSTEMC_HOME}
                PATH_SUFFIXES lib lib32 lib64 lib-linux lib-linux32 lib-linux64 lib-macosx64
                lib-cygwin lib-cygwin32 lib-cygwin64
<<<<<<< HEAD
                DOC "Path to the UVM SystemC library"
                )
=======
            DOC "Path to the UVM SystemC library"
        )
        message(STATUS ${UVM_SYSTEMC_LIBRARY})
>>>>>>> 00fbf84 (refactor: change the structure)

        if (UVM_SYSTEMC_LIBRARY)
            add_library(uvm-systemc UNKNOWN IMPORTED)
            set_target_properties(uvm-systemc PROPERTIES IMPORTED_LOCATION
                    ${UVM_SYSTEMC_LIBRARY})
        endif ()

        mark_as_advanced(UVM_SYSTEMC_LIBRARY)

        if ("${CMAKE_VERSION}" VERSION_GREATER 2.8.12)
            get_filename_component(UVM_SYSTEMC_LIBRARY_DIR ${UVM_SYSTEMC_LIBRARY}
                    DIRECTORY)
        endif ()
        mark_as_advanced(UVM_SYSTEMC_LIBRARY_DIR)

        find_path(UVM_SYSTEMC_INCLUDE_DIR uvm.h
                HINTS $ENV{UVM_SYSTEMC_INCLUDE} $ENV{SYSTEMC_INCLUDE}
                $ENV{SYSTEMC_HOME}/include
                DOC "Path to the UVM SystemC headers"
                )

        mark_as_advanced(UVM_SYSTEMC_INCLUDE_DIR)

        find_package_handle_standard_args(UVM_SystemC REQUIRED_VARS
                UVM_SYSTEMC_LIBRARY_DIR UVM_SYSTEMC_LIBRARY UVM_SYSTEMC_INCLUDE_DIR)

        if (UVM_SYSTEMC_FOUND)
            set(SYSTEMC_LIBRARY_DIRS ${SYSTEMC_LIBRARY_DIRS} ${SYSTEMC_LIBRARY_DIR} ${UVM_SYSTEMC_LIBRARY_DIR})
            set(SYSTEMC_LIBRARIES ${SYSTEMC_LIBRARIES} ${SYSTEMC_LIBRARY} uvm-systemc)
            set(SYSTEMC_INCLUDE_DIRS ${SYSTEMC_INCLUDE_DIRS} ${UVM_SYSTEMC_INCLUDE_DIR})
        else ()
            set(SYSTEMC_FOUND FALSE)
        endif ()
    endmacro()

    foreach (component ${SystemC_FIND_COMPONENTS})
        if (component STREQUAL "SCV")
            find_systemc_component_scv()
        elseif (component STREQUAL "UVM")
            find_systemc_component_uvm()
        endif ()
    endforeach ()

    #    if (SYSTEMC_FOUND)
    #        if (WIN32)
    #            set(library_policy STATIC)
    #        else ()
    #            set(library_policy SHARED)
    #        endif ()
    #
    #        add_library(systemc-main ${library_policy}
    #                ${CMAKE_CURRENT_LIST_DIR}/sc_main.cpp
    #                )
    #
    #        set_target_properties(systemc-main PROPERTIES
    #                ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    #                LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    #                RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    #                )
    #
    #        #         logic_target_compile_options(systemc-main)
    #        target_compile_options(systemc-main PRIVATE -fPIC -flto -std=c++2a
    #                -pedantic
    #                -fstrict-aliasing
    #                -Weverything
    #                -Wno-padded
    #                -Wno-covered-switch-default
    #                -Wno-c++98-compat
    #                -Wno-c++98-compat-pedantic)
    #
    #        target_include_directories(systemc-main SYSTEM
    #                PRIVATE ${SYSTEMC_INCLUDE_DIR})
    #        target_link_libraries(systemc-main PRIVATE ${SYSTEMC_LIBRARIES})
    #
    #        #        set(SYSTEMC_LIBRARIES systemc-main ${SYSTEMC_LIBRARIES} systemc)
    #        set(SYSTEMC_INCLUDE_DIRS ${SYSTEMC_INCLUDE_DIRS} ${SYSTEMC_INCLUDE_DIR})
    #    endif ()

    list(REMOVE_DUPLICATES SYSTEMC_LIBRARY_DIRS)
    list(REMOVE_DUPLICATES SYSTEMC_LIBRARIES)
    list(REMOVE_DUPLICATES SYSTEMC_INCLUDE_DIRS)

    set(SYSTEMC_FOUND ${SYSTEMC_FOUND} PARENT_SCOPE)
    set(SYSTEMC_LIBRARY_DIRS ${SYSTEMC_LIBRARY_DIRS} PARENT_SCOPE)
    set(SYSTEMC_LIBRARIES ${SYSTEMC_LIBRARIES} PARENT_SCOPE)
    set(SYSTEMC_INCLUDE_DIRS ${SYSTEMC_INCLUDE_DIRS} PARENT_SCOPE)
endfunction()

_find_systemc()
