cmake_minimum_required(VERSION 3.28.3)
project(file_sys C)

set(CMAKE_C_STANDARD 11)

get_filename_component(parent_dir ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)


# 1. include utility functions
set(cmake_utils "${parent_dir}/cmake_utils/proj_utils.cmake")
include(${cmake_utils})

# 2. add /src that adds .c.h files and adds other sub-directories
add_subdirectory(src)
#add_subdirectory(test)

include_project(${parent_dir}/g_includes)
include_project(${parent_dir}/logger/src)
include_project(${parent_dir}/memory/src)
include_project(${parent_dir}/memory/src/mem_log)
include_project(${parent_dir}/memory/src/internal)
#include_project(${parent_dir}/perf_timer/src)

# 3. create executable with all the source files
add_project_executable(file_sys main.c)

# Debug: global properties
print_all_registered()