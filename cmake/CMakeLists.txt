cmake_minimum_required(VERSION 3.28.3)
project(cmake C)

set(CMAKE_C_STANDARD 11)

get_filename_component(parent_dir ${CMAKE_SOURCE_DIR} DIRECTORY)

# 1. include utility functions
include(${parent_dir}/cmake_utils/proj_utils.cmake)

# 2. add /src that adds .c.h files and adds other sub-directories
add_subdirectory(src)

# 3. create executable with all the source files
add_project_executable(cmake main.c)

# Debug: global properties
print_all_registered()