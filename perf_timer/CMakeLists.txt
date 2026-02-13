cmake_minimum_required(VERSION 3.28.3)
project(perf_timer C)

set(CMAKE_C_STANDARD 11)

get_filename_component(parent_dir ${CMAKE_SOURCE_DIR} DIRECTORY)

# 1. include utility functions
set(cmake_utils "${parent_dir}/cmake_utils/proj_utils.cmake")
include(${cmake_utils})

# 2. add /src that adds .c.h files and adds other sub-directories
add_subdirectory(src)


include_project(${parent_dir}/g_includes)
include_project(${parent_dir}/logger/src)
include_project(${parent_dir}/buffer/src)

# 3. create executable with all the source files
add_project_executable(perf_timer main.c)

# Debug: global properties
print_all_registered()


#
#cmake_minimum_required(VERSION 4.1)
#project(perf_timer C)
#
#set(CMAKE_C_STANDARD 11)
#
#include("${CMAKE_CURRENT_SOURCE_DIR}/src/CMakeLists.txt")
#
################## external directories #################
#get_filename_component(parent_dir ${CMAKE_SOURCE_DIR} DIRECTORY)
#include("${parent_dir}/g_includes/CMakeLists.txt")
#include("${parent_dir}/logger/src/CMakeLists.txt")
#include("${parent_dir}/memory/src/CMakeLists.txt")
#include("${parent_dir}/memory/src/mem_log/CMakeLists.txt")
#include("${parent_dir}/allocators/src/slab_allocator/CMakeLists.txt")
#include("${parent_dir}/allocators/src/slab_allocator/slab_allocator_log/CMakeLists.txt")
#include("${parent_dir}/buffer/src/CMakeLists.txt")
#include("${parent_dir}/buffer/src/buffer_logger/CMakeLists.txt")
#set(external_include
#        ${g_includes}
#        ${logger_include}
#        ${mms_include}
#        ${mms_loger_include}
#        ${slab_allocator_include}
#        ${slab_allocator_log_include}
#        ${buffer_include}
#        ${buffer_log_include}
#)
#set(external_source
#        ${sys_utils}
#        ${bitfields}
#        ${logger_source}
#        ${mms_source}
#        ${mms_loger_source}
#        ${slab_allocator_source}
#        ${slab_allocator_log_source}
#        ${buffer_source}
#        ${buffer_log_source}
#)
#
#add_executable(perf_timer main.c
#        ${precision_timer_source}
#        ${external_source}
#
#)
#target_include_directories(perf_timer PRIVATE
#        ${precision_timer_include}
#        ${external_include}
#)
