cmake_minimum_required(VERSION 3.28.3)

set(CMAKE_C_STANDARD 11)

# 1. include utility functions
include(${cmake_utils})

# 2. add /src that adds .c.h files and adds other sub-directories
#add_subdirectory(src)

include_project(${parent_dir}/g_includes)
include_project(${parent_dir}/logger/src)
include_project(${parent_dir}/perf_timer/src)

# 3. create executable with all the source files
add_project_executable(tests main.c)
#add_executable(tests main.c)

# Debug: global properties
print_all_registered()



#set(project_dir "${CMAKE_SOURCE_DIR}")
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
#add_executable(tests
#        main.c
#        tests.c
#        ${mms_source}
#        ${mms_internal_source}
#        ${mms_logger_source}
#        ${external_source}
#)
#target_include_directories(tests PRIVATE
#        ${mms_include}
#        ${mms_internal_include}
#        ${mms_logger_include}
#        ${external_include}
#)

#include(/home/ckalo/data/projects/clion/cmake_utils/proj_utils.cmake)
#
#register_module(
#        SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/tests.c
#        HEADERS
#        ${CMAKE_CURRENT_SOURCE_DIR}/tests.h
#        INCLUDE_DIRS
#        ${CMAKE_CURRENT_SOURCE_DIR}
#        #        MODULE=memory
#        ENABLE_DEBUG=1
#        VERSION_MAJOR=0
#)
#
#add_subdirectory(${project_dir}/src)
#
#add_executable(tests main.c ${ALL_SOURCES})
#
#target_include_directories(tests PRIVATE ${ALL_INCLUDE_DIRS})
#
### Debug: global properties
#get_property(ALL_SOURCES GLOBAL PROPERTY GLOBAL_SOURCES)
#get_property(ALL_HEADERS GLOBAL PROPERTY GLOBAL_HEADERS)
#get_property(ALL_INCLUDE_DIRS GLOBAL PROPERTY GLOBAL_INCLUDE_DIRS)
#message("Sources: ${ALL_SOURCES}")
#message("Headers: ${ALL_HEADERS}")
#message("Include dirs: ${ALL_INCLUDE_DIRS}")