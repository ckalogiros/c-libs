include(${cmake_utils})

register_module(
        sources
        ${CMAKE_CURRENT_LIST_DIR}/stack_allocator.c
        headers
        ${CMAKE_CURRENT_LIST_DIR}/stack_allocator_struct.h
        ${CMAKE_CURRENT_LIST_DIR}/stack_allocator.h
        include_dirs
        ${CMAKE_CURRENT_LIST_DIR}
        compiler_macros
        ENABLE_DEBUG=1
        VERSION_MAJOR=0
)

