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


#cmake_minimum_required(VERSION 4.1)
#project(logger C)
#
#set(CMAKE_C_STANDARD 11)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
#
#set(project_dir "${CMAKE_SOURCE_DIR}")
#include("${project_dir}/src/CMakeLists.txt")
#include("${project_dir}/src/to_html/CMakeLists.txt")
#
#
################## external directories #################
#get_filename_component(parent_dir ${CMAKE_SOURCE_DIR} DIRECTORY)
#include("${parent_dir}/g_includes/CMakeLists.txt")
#set(external_include
#        ${g_includes}
#)
#set(external_source
#        ${sys_utils}
#)
#
#add_executable(logger main.c
#        ${logger_source}
#        ${logger_to_html_source}
#        ${external_source}
#)
#target_include_directories(logger PRIVATE
#        ${logger_include}
#        ${logger_to_html_include}
#        ${external_include}
#)
