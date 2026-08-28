include(${cmake_utils})

register_module(
        sources
        ${CMAKE_CURRENT_LIST_DIR}/arena_allocator.c
        headers
        ${CMAKE_CURRENT_LIST_DIR}/arena_allocator_types.h
        ${CMAKE_CURRENT_LIST_DIR}/arena_allocator_struct.h
        ${CMAKE_CURRENT_LIST_DIR}/arena_allocator_macros.h
        ${CMAKE_CURRENT_LIST_DIR}/arena_allocator.h
        include_dirs
        ${CMAKE_CURRENT_LIST_DIR}
        compiler_macros
        ENABLE_DEBUG=1
        VERSION_MAJOR=0
)

