include(${cmake_utils})
register_module(
        sources
            ${CMAKE_CURRENT_LIST_DIR}/tests.c
            ${CMAKE_CURRENT_LIST_DIR}/mem_unit_test.c
        headers
            ${CMAKE_CURRENT_LIST_DIR}/tests.h
            ${CMAKE_CURRENT_LIST_DIR}/mem_unit_test.h
        include_dirs
            ${CMAKE_CURRENT_LIST_DIR}
        compiler_macros
            ENABLE_DEBUG=1
            VERSION_MAJOR=0
)

