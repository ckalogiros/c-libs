cmake_minimum_required(VERSION 4.1)
project(renderer C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O3")

set(project_dir "${CMAKE_SOURCE_DIR}")
include("${project_dir}/src/renderer/CMakeLists.txt")
include("${project_dir}/src/window/CMakeLists.txt")
include("${project_dir}/src/graphics/CMakeLists.txt")
add_subdirectory(${project_dir}/tests)


################# external directories #################
get_filename_component(parent_dir ${CMAKE_SOURCE_DIR} DIRECTORY)
include("${parent_dir}/g_includes/CMakeLists.txt")
#include("${parent_dir}/perf_timer/src/CMakeLists.txt")
#include("${parent_dir}/logger/src/CMakeLists.txt")
#include("${parent_dir}/memory/src/CMakeLists.txt")
#include("${parent_dir}/memory/src/mem_log/CMakeLists.txt")
#include("${parent_dir}/allocators/src/slab_allocator/CMakeLists.txt")
#include("${parent_dir}/allocators/src/slab_allocator/slab_allocator_log/CMakeLists.txt")
#include("${parent_dir}/buffer/src/CMakeLists.txt")
#include("${parent_dir}/buffer/src/buffer_logger/CMakeLists.txt")
include("/home/ckalo/data/projects/third_party/glad/src/CMakeLists.txt")
set(external_include
        ${g_includes}
        #        ${precision_timer_include}
#        ${logger_include}
        #        ${mms_include}
        #        ${slab_allocator_include}
        #        ${slab_allocator_log_include}
        #        ${mms_loger_include}
        #        ${buffer_include}
        #        ${buffer_log_include}
#        ${glad_include}
)
set(external_source
        ${sys_utils}
        #        ${bitfields}
        #        ${precision_timer_source}
#        ${logger_source}
        #        ${mms_source}
        #        ${slab_allocator_source}
        #        ${slab_allocator_log_source}
        #        ${mms_loger_source}
        #        ${buffer_source}
        #        ${buffer_log_source}
#        ${glad_source}
)

add_executable(renderer main.c
#        ${renderer_source}
#        ${window_source}
#        ${graphics_source}
#        ${external_source}
)
target_include_directories(renderer PRIVATE
#        ${renderer_include}
#        ${window_include}
#        ${graphics_include}
#        ${external_include}
)

# Link X11 lib
target_link_libraries(renderer X11 GL)

