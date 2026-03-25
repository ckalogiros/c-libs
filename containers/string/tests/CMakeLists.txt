include(${cmake_utils})

register_module(
        sources
            ${CMAKE_CURRENT_LIST_DIR}/test_performance.c
            ${CMAKE_CURRENT_LIST_DIR}/tests_other.c
        headers
            ${CMAKE_CURRENT_LIST_DIR}/test_performance.h
            ${CMAKE_CURRENT_LIST_DIR}/tests_other.h
        include_dirs
            ${CMAKE_CURRENT_LIST_DIR}
        compiler_macros
            ENABLE_DEBUG=1
            VERSION_MAJOR=0
)
