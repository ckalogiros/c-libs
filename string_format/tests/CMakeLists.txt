cmake_minimum_required(VERSION 3.28)

set(CMAKE_C_STANDARD 11)

get_filename_component(parent_dir ${CMAKE_SOURCE_DIR} DIRECTORY)

# 1. include utility functions
set(cmake_utils "${parent_dir}/cmake_utils/proj_utils.cmake")
include(${cmake_utils})

# 2. add /src that adds .c.h files and adds other sub-directories
include(${CMAKE_SOURCE_DIR}/src/CMakeLists.txt)

include_project(${parent_dir}/g_includes)
include_project(${parent_dir}/logger/src)
include_project(${parent_dir}/file_sys/src)
include_project(${parent_dir}/memory/src)
include_project(${parent_dir}/memory/src/mem_log)
include_project(${parent_dir}/memory/src/internal)
include_project(${parent_dir}/buffer/src)
include_project(${parent_dir}/buffer/src/buffer_logger)
include_project(${parent_dir}/allocators/src/slab_allocator)
include_project(${parent_dir}/allocators/src/slab_allocator/slab_allocator_log)
include_project(${parent_dir}/string/src)
include_project(${parent_dir}/string/src/string_log)
include_project(${parent_dir}/allocators/src/string_allocator)
include_project(${parent_dir}/allocators/src/string_allocator/string_allocator_log)
include_project(${parent_dir}/perf_timer/src)

# 3. create executable with all the source files
add_project_executable(tests main.c)
#add_executable(tests main.c)


# Debug: global properties
print_all_registered()

