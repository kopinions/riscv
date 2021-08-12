include(FetchContent)
FetchContent_Declare(SystemC
        URL https://www.accellera.org/images/downloads/standards/systemc/systemc-2.3.3.tar.gz)
FetchContent_GetProperties(SystemC)
if (NOT systemc_POPULATED)
    FetchContent_Populate(SystemC)
    add_subdirectory(${systemc_SOURCE_DIR})
endif ()