cmake_minimum_required(VERSION 3.28.3)

set(CMAKE_C_STANDARD 11)

# 1. include utility functions
include(${cmake_utils})

# 2. add other sub-directories files
register_module(
        sources
        ${CMAKE_CURRENT_LIST_DIR}/test_unit_bits.c
        ${CMAKE_CURRENT_LIST_DIR}/test_unit_bitfields.c
        headers
        ${CMAKE_CURRENT_LIST_DIR}/test_unit_bits.h
        ${CMAKE_CURRENT_LIST_DIR}/test_unit_bitfields.h
        include_dirs
        ${CMAKE_CURRENT_LIST_DIR}
)

include_project(${parent_dir}/g_includes)
include_project(${parent_dir}/logger/src)
include_project(${parent_dir}/file_sys/src)
include_project(${parent_dir}/string_format/src)
include_project(${parent_dir}/memory/src)
include_project(${parent_dir}/memory/src/mem_log)
include_project(${parent_dir}/memory/src/internal)
include_project(${parent_dir}/buffer/src)
include_project(${parent_dir}/buffer/src/buffer_log)
include_project(${parent_dir}/allocators/src/slab_allocator)
include_project(${parent_dir}/allocators/src/slab_allocator/slab_allocator_log)
include_project(${parent_dir}/allocators/src/stack_allocator)
include_project(${parent_dir}/string/src)
include_project(${parent_dir}/string/src/string_log)
include_project(${parent_dir}/perf_timer/src)

# 3. create executable with all the source files
add_project_executable(tests main.c)
#add_executable(tests main.c)

# Debug: global properties
print_all_registered()