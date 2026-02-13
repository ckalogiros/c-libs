include(${cmake_utils})

register_module(
        sources
        ${CMAKE_CURRENT_LIST_DIR}/mem_cache.c
        ${CMAKE_CURRENT_LIST_DIR}/slab_allocator_internals.c
        headers
        ${CMAKE_CURRENT_LIST_DIR}/mem_cache.h
        ${CMAKE_CURRENT_LIST_DIR}/slab_allocator_internals.h
        ${CMAKE_CURRENT_LIST_DIR}/slab_allocator_struct.h
        include_dirs
        ${CMAKE_CURRENT_LIST_DIR}
        compiler_macros
        ENABLE_DEBUG=1
        VERSION_MAJOR=0
)
