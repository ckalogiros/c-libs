cmake_minimum_required(VERSION 3.28.3)
project(string C)

set(CMAKE_C_STANDARD 11)

get_filename_component(parent_dir ${CMAKE_SOURCE_DIR} DIRECTORY)

# 1. include utility functions
set(cmake_utils "${parent_dir}/cmake_utils/proj_utils.cmake")
include(${cmake_utils})

# 2. add /src that adds .c.h files and adds other sub-directories
include(${CMAKE_SOURCE_DIR}/src/CMakeLists.txt)
include(${CMAKE_SOURCE_DIR}/src/string_log/CMakeLists.txt)

add_subdirectory(tests)

include_project(${parent_dir}/g_includes)
include_project(${parent_dir}/logger/src)
include_project(${parent_dir}/file_sys/src)
include_project(${parent_dir}/memory/src)
include_project(${parent_dir}/memory/src/mem_log)
include_project(${parent_dir}/memory/src/internal)
include_project(${parent_dir}/buffer/src)
include_project(${parent_dir}/buffer/src/buffer_log)
include_project(${parent_dir}/allocators/src/slab_allocator)
include_project(${parent_dir}/allocators/src/slab_allocator/slab_allocator_log)
include_project(${parent_dir}/allocators/src/stack_allocator)
#include_project(${parent_dir}/allocators/src/stack_allocator/stack_allocator_log)
include_project(${parent_dir}/perf_timer/src)

# 3. create executable with all the source files
add_project_executable(string main.c)
#add_executable(buffer main.c)


# Debug: global properties
print_all_registered()
