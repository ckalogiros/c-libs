cmake_minimum_required(VERSION 4.1)
project(html_gen C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O3")

set(project_dir "${CMAKE_SOURCE_DIR}")
#include("${project_dir}/data_oriented/CMakeLists.txt")
#add_subdirectory("${project_dir}/tests")



set(project_dir "${CMAKE_SOURCE_DIR}")
include("${project_dir}/src/CMakeLists.txt")
#include("${project_dir}/src/buffer_logger/CMakeLists.txt")


################# external directories #################
get_filename_component(parent_dir ${CMAKE_SOURCE_DIR} DIRECTORY)
include("${parent_dir}/g_includes/CMakeLists.txt")
include("${parent_dir}/perf_timer/src/CMakeLists.txt")
include("${parent_dir}/logger/src/CMakeLists.txt")
include("${parent_dir}/memory/src/CMakeLists.txt")
include("${parent_dir}/memory/src/mem_log/CMakeLists.txt")
include("${parent_dir}/allocators/src/slab_allocator/CMakeLists.txt")
include("${parent_dir}/allocators/src/slab_allocator/slab_allocator_log/CMakeLists.txt")
include("${parent_dir}/buffer/src/CMakeLists.txt")
include("${parent_dir}/buffer/src/buffer_logger/CMakeLists.txt")
set(external_include
        ${g_includes}
        ${precision_timer_include}
        ${logger_include}
        ${mms_include}
        ${mms_loger_include}
        ${slab_allocator_include}
        ${slab_allocator_log_include}
        ${buffer_include}
        ${buffer_log_include}

)
set(external_source
        ${sys_utils}
        ${bitfields}
        ${precision_timer_source}
        ${logger_source}
        ${mms_source}
        ${mms_loger_source}
        ${slab_allocator_source}
        ${slab_allocator_log_source}
        ${buffer_source}
        ${buffer_log_source}
)

add_executable(html_gen main.c
        ${html_gen_source}
        ${external_source}
)
target_include_directories(html_gen PRIVATE
        ${html_gen_include}
        ${external_include}
)

