cmake_minimum_required(VERSION 3.28.3)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

get_filename_component(parent_dir ${CMAKE_SOURCE_DIR} DIRECTORY)

# 1. include utility functions
set(cmake_utils "${parent_dir}/cmake_utils/proj_utils.cmake")
include(${cmake_utils})

# 2. add /src that adds .c.h files and adds other sub-directories
include(${CMAKE_SOURCE_DIR}/src/slab_allocator/CMakeLists.txt)
include(${CMAKE_SOURCE_DIR}/src/slab_allocator/slab_allocator_log/CMakeLists.txt)

register_module(
        sources
        ${CMAKE_CURRENT_LIST_DIR}/test_performance.c
        ${CMAKE_CURRENT_LIST_DIR}/test_slab_allocator.c
        ${CMAKE_CURRENT_LIST_DIR}/test_stack_allocator.c
        headers
        ${CMAKE_CURRENT_LIST_DIR}/test_performance.h
        ${CMAKE_CURRENT_LIST_DIR}/test_slab_allocator.h
        ${CMAKE_CURRENT_LIST_DIR}/test_stack_allocator.h
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
include_project(${parent_dir}/string/src)
include_project(${parent_dir}/string/src/string_log)
include_project(${parent_dir}/allocators/src/slab_allocator/slab_allocator_log)
include_project(${parent_dir}/perf_timer/src)


# 3. create executable with all the source files
add_project_executable(tests main.c)


# Debug: global properties
print_all_registered()
