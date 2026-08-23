include(${cmake_utils})

register_module(
        sources
        ${CMAKE_CURRENT_LIST_DIR}/test_buffers.c
        ${CMAKE_CURRENT_LIST_DIR}/unrolled_loop_access.c
        ${CMAKE_CURRENT_LIST_DIR}/test_code_for_assembly.c
        headers
        ${CMAKE_CURRENT_LIST_DIR}/test_buffers.h
        ${CMAKE_CURRENT_LIST_DIR}/unrolled_loop_access.h
        include_dirs
        ${CMAKE_CURRENT_LIST_DIR}
        compiler_macros
        ENABLE_DEBUG=1
        VERSION_MAJOR=0
)