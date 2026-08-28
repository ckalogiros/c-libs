include(${cmake_utils})

register_module(
        sources
            ${CMAKE_CURRENT_LIST_DIR}/slab_allocator_log.c
            ${CMAKE_CURRENT_LIST_DIR}/slab_allocator_fprint.c
        headers
            ${CMAKE_CURRENT_LIST_DIR}/slab_allocator_log.h
        ${CMAKE_CURRENT_LIST_DIR}/slab_allocator_fprint.h
        include_dirs
            ${CMAKE_CURRENT_LIST_DIR}
        compiler_macros
            ENABLE_DEBUG=1
            VERSION_MAJOR=0
)
