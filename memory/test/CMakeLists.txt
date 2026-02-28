cmake_minimum_required(VERSION 3.28.3)

set(CMAKE_C_STANDARD 11)

# 1. include utility functions
include(${cmake_utils})

# 2. add other sub-directories files
include(${CMAKE_SOURCE_DIR}/src/CMakeLists.txt)
include(${CMAKE_SOURCE_DIR}/src/internal/CMakeLists.txt)
include(${CMAKE_SOURCE_DIR}/src/mem_log/CMakeLists.txt)
register_module(
        sources
        ${CMAKE_CURRENT_LIST_DIR}/tests.c
        headers
        ${CMAKE_CURRENT_LIST_DIR}/tests.h
        include_dirs
        ${CMAKE_CURRENT_LIST_DIR}
)

include_project(${parent_dir}/g_includes)
include_project(${parent_dir}/logger/src)
include_project(${parent_dir}/file_sys/src)
include_project(${parent_dir}/allocators/src/slab_allocator)
include_project(${parent_dir}/allocators/src/slab_allocator/slab_allocator_log)
include_project(${parent_dir}/buffer/src)
include_project(${parent_dir}/buffer/src/buffer_logger)
include_project(${parent_dir}/perf_timer/src)

# 3. create executable with all the source files
add_project_executable(tests main.c)
#add_executable(tests main.c)

# Debug: global properties
print_all_registered()

