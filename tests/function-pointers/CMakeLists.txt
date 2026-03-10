cmake_minimum_required(VERSION 3.28.3)

set(CMAKE_C_STANDARD 11)

get_filename_component(parent_dir ${CMAKE_SOURCE_DIR} DIRECTORY)

# 1. include utility functions
set(cmake_utils "${parent_dir}/cmake_utils/proj_utils.cmake")
include(${cmake_utils})

# 2. add /src that adds .c.h files and adds other sub-directories
#include(${CMAKE_SOURCE_DIR}/src/CMakeLists.txt)
register_module(
        sources
#        ${CMAKE_CURRENT_LIST_DIR}/test.c
        headers
#        ${CMAKE_CURRENT_LIST_DIR}/test.h
        include_dirs
        ${CMAKE_CURRENT_LIST_DIR}
        compiler_macros
        ENABLE_DEBUG=1
        VERSION_MAJOR=0
)

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
include_project(${parent_dir}/perf_timer/src)

# 3. create executable with all the source files
add_project_executable(function_pointers function_pointers_main.c)

# Link the math library (libm) to the "math_app" executable
target_link_libraries(function_pointers m)  # "m" is the linker name for libm

# Debug: global properties
print_all_registered()
