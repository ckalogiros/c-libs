cmake_minimum_required(VERSION 4.1)
project(memory C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")


include(/home/ckalo/data/projects/clion/cmake_utils/proj_utils.cmake)

add_subdirectory(src)

add_executable(memory main.c ${ALL_SOURCES})

target_include_directories(memory PRIVATE ${ALL_INCLUDE_DIRS})

## Debug: global properties
get_property(ALL_SOURCES GLOBAL PROPERTY GLOBAL_SOURCES)
get_property(ALL_HEADERS GLOBAL PROPERTY GLOBAL_HEADERS)
get_property(ALL_INCLUDE_DIRS GLOBAL PROPERTY GLOBAL_INCLUDE_DIRS)

print_all_registered()


#set(project_dir "${CMAKE_SOURCE_DIR}")
#add_subdirectory(${project_dir}/test)
#include("${project_dir}/src/CMakeLists.txt")
#include("${project_dir}/src/internal/CMakeLists.txt")
#include("${project_dir}/src/mem_log/CMakeLists.txt")

#
################## external directories #################
#get_filename_component(parent_dir ${CMAKE_SOURCE_DIR} DIRECTORY)
#include("${parent_dir}/g_includes/CMakeLists.txt")
#include("${parent_dir}/perf_timer/src/CMakeLists.txt")
#include("${parent_dir}/logger/src/CMakeLists.txt")
#include("${parent_dir}/logger/src/to_html/CMakeLists.txt")
#include("${parent_dir}/allocators/src/slab_allocator/CMakeLists.txt")
#include("${parent_dir}/allocators/src/slab_allocator/slab_allocator_log/CMakeLists.txt")
#include("${parent_dir}/buffer/src/CMakeLists.txt")
#include("${parent_dir}/buffer/src/buffer_logger/CMakeLists.txt")
#
#set(external_include
#        ${g_includes}
#        ${logger_include}
#        ${logger_to_html_include}
#        ${precision_timer_include}
#        ${slab_allocator_include}
#        ${slab_allocator_log_include}
#        ${buffer_include}
#        ${buffer_log_include}
#)
#set(external_source
#        ${sys_utils}
#        ${bitfields}
#        ${logger_source}
#        ${logger_to_html_source}
#        ${precision_timer_source}
#        ${slab_allocator_source}
#        ${slab_allocator_log_source}
#        ${buffer_source}
#        ${buffer_log_source}
#)
#
#add_executable(memory main.c
#        ${mms_source}
#        ${mms_loger_source}
#        ${external_source}
#)
#target_include_directories(memory PRIVATE
#        ${mms_include}
#        ${mms_internal_include}
#        ${mms_loger_include}
#        ${external_include}
#)
#
