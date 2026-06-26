include(${cmake_utils})

register_module(
        sources
        ${CMAKE_CURRENT_LIST_DIR}/unit_test.c
        ${CMAKE_CURRENT_LIST_DIR}/tests_other.c
        headers
        ${CMAKE_CURRENT_LIST_DIR}/unit_test.h
        ${CMAKE_CURRENT_LIST_DIR}/tests_other.h
        include_dirs
        ${CMAKE_CURRENT_LIST_DIR}
        compiler_macros
        ENABLE_DEBUG=1
        VERSION_MAJOR=0
)
