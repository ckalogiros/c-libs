cmake_minimum_required(VERSION 3.28.3)
project(allocators C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

get_filename_component(parent_dir ${CMAKE_SOURCE_DIR} DIRECTORY)

# 1. include utility functions
set(cmake_utils "${parent_dir}/cmake_utils/proj_utils.cmake")
include(${cmake_utils})

# 2. add /src that adds .c.h files and adds other sub-directories
add_subdirectory(src/slab_allocator)
add_subdirectory(tests)


include_project(${parent_dir}/g_includes)
include_project(${parent_dir}/logger/src)
include_project(${parent_dir}/memory/src)
include_project(${parent_dir}/memory/src/mem_log)
include_project(${parent_dir}/memory/src/internal)

include(${cmake_utils})



# 3. create executable with all the source files
add_project_executable(allocators main.c)
#add_executable(perf_timer main.c)

# Debug: global properties
print_all_registered()



#cmake_minimum_required(VERSION 4.1)
#project(allocators C)
#
#set(CMAKE_C_STANDARD 11)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
#
#set(project_dir "${CMAKE_SOURCE_DIR}")
#include("${project_dir}/src/slab_allocator/CMakeLists.txt")
#include("${project_dir}/src/slab_allocator/slab_allocator_log/CMakeLists.txt")
#include("${project_dir}/src/stack_allocator/CMakeLists.txt")
#include("${project_dir}/src/generic_allocator/CMakeLists.txt")
#add_subdirectory(${project_dir}/tests)

#
################## external directories #################
#get_filename_component(parent_dir ${CMAKE_SOURCE_DIR} DIRECTORY)
#include("${parent_dir}/g_includes/CMakeLists.txt")
#include("${parent_dir}/logger/src/CMakeLists.txt")
##include("${parent_dir}/perf_timer/src/CMakeLists.txt")
##include("${parent_dir}/memory/src/CMakeLists.txt")
##include("${parent_dir}/memory/src/internal/CMakeLists.txt")
##include("${parent_dir}/memory/src/mem_log/CMakeLists.txt")
##include("${parent_dir}/buffer/src/CMakeLists.txt")
##include("${parent_dir}/buffer/src/buffer_logger/CMakeLists.txt")
##include("${parent_dir}/io/src/std_input/CMakeLists.txt")
#
#set(external_include
#        ${g_includes}
#        ${logger_include}
##        ${mms_source}
##        ${mms_internal_source}
##        ${mms_logger_source}
##        ${precision_timer_include}
##        ${buffer_include}
##        ${buffer_log_include}
##        ${std_input_include}
#)
#set(external_source
#        ${sys_utils}
#        ${bitfields}
#        ${logger_source}
##        ${mms_source}
##        ${mms_internal_source}
##        ${mms_logger_source}
##        ${precision_timer_source}
##        ${buffer_source}
##        ${buffer_log_source}
##        ${std_input_source}
#)
#
#add_executable(allocators main.c
#        ${external_source}
#        ${allocators_source}
#        ${slab_allocator_source}
#        ${slab_allocator_log_source}
#)
#target_include_directories(allocators PRIVATE
#        ${external_include}
#        ${allocators_include}
#        ${slab_allocator_include}
#        ${slab_allocator_log_include}
#)
#
