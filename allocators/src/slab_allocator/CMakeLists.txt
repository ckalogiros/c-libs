# include dir
set(slab_allocator_include "${CMAKE_CURRENT_LIST_DIR}/")
# bundle source files
set(slab_allocator_source
        "${CMAKE_CURRENT_LIST_DIR}/mem_cache.c"
        "${CMAKE_CURRENT_LIST_DIR}/slab_allocator_internals.c"
)

message(slab_allocator_source="${slab_allocator_source}")

