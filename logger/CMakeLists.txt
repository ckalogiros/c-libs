cmake_minimum_required(VERSION 3.28.3)
project(logger C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

get_filename_component(parent_dir ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)

# 1. include utility functions
set(cmake_utils "${parent_dir}/cmake_utils/proj_utils.cmake")
include(${cmake_utils})

# 2. add /src that adds .c.h filees and adds other sub-directories
add_subdirectory(src)
include_project(${parent_dir}/g_includes)

# 3. create executable with all the source files
add_project_executable(logger main.c)

# Debug: global properties
print_all_registered()
