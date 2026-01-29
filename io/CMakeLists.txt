cmake_minimum_required(VERSION 4.1)
project(io C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

set(project_dir "${CMAKE_SOURCE_DIR}")
include("${project_dir}/src/std_input/CMakeLists.txt")
#include("${project_dir}/src/slab_allocator/slab_allocator_log/CMakeLists.txt")

#add_subdirectory(${project_dir}/tests)


################# external directories #################
get_filename_component(parent_dir ${CMAKE_SOURCE_DIR} DIRECTORY)
include("${parent_dir}/g_includes/CMakeLists.txt")
include("${parent_dir}/logger/src/CMakeLists.txt")
#include("${parent_dir}/perf_timer/src/CMakeLists.txt")
#include("${parent_dir}/memory/src/CMakeLists.txt")
#include("${parent_dir}/memory/src/mem_log/CMakeLists.txt")

set(external_include
        ${g_includes}
        ${logger_include}
#        ${precision_timer_include}
)
set(external_source
        ${sys_utils}
        ${logger_source}
#        ${precision_timer_source}
)

add_executable(io main.c
        ${std_input_source}
#        ${allocators_source}
#        ${slab_allocator_source}
#        ${slab_allocator_log_source}
#        ${mms_source}
#        ${mms_loger_source}
        ${external_source}
)
target_include_directories(io PRIVATE
        ${std_input_include}
#        ${allocators_include}
#        ${slab_allocator_include}
#        ${slab_allocator_log_include}
#        ${mms_include}
#        ${mms_loger_include}
        ${external_include}
)
