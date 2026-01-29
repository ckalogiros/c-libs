cmake_minimum_required(VERSION 4.1)
project(list C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

set(project_dir "${CMAKE_SOURCE_DIR}")
include("${project_dir}/src/CMakeLists.txt")
add_subdirectory(${project_dir}/tests)


################# external directories #################
get_filename_component(parent_dir ${CMAKE_SOURCE_DIR} DIRECTORY)
include("${parent_dir}/g_includes/CMakeLists.txt")
include("${parent_dir}/perf_timer/src/CMakeLists.txt")
include("${parent_dir}/logger/src/CMakeLists.txt")
include("${parent_dir}/memory/src/CMakeLists.txt")
include("${parent_dir}/memory/src/allocators/CMakeLists.txt")
include("${parent_dir}/memory/src/slab_allocator/CMakeLists.txt")
include("${parent_dir}/memory/src/mem_log/CMakeLists.txt")
set(external_include
        ${g_includes}
        ${logger_include}
        ${precision_timer_include}
        ${mms_include}
        ${allocators_include}
        ${mms_loger_include}
#        ${slab_allocator_include}
)
set(external_source
        ${sys_utils}
        ${logger_source}
        ${precision_timer_source}
        ${mms_source}
        ${allocators_source}
        ${mms_loger_source}
#        ${slab_allocator_source}
)

add_executable(list main.c
        ${list_source}
        ${external_source}
)
target_include_directories(list PRIVATE
        ${list_include}
        ${external_include}
)

