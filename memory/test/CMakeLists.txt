set(project_dir "${CMAKE_SOURCE_DIR}")
include("${project_dir}/src/CMakeLists.txt")
include("${project_dir}/src/mem_log/CMakeLists.txt")
#include("${project_dir}/src/slab_allocator/CMakeLists.txt")
#include("${project_dir}/src/slab_allocator/slab_allocator_log/CMakeLists.txt")

################# external directories #################
get_filename_component(parent_dir ${CMAKE_SOURCE_DIR} DIRECTORY)
include("${parent_dir}/g_includes/CMakeLists.txt")
include("${parent_dir}/perf_timer/src/CMakeLists.txt")
include("${parent_dir}/logger/src/CMakeLists.txt")
include("${parent_dir}/list/src/CMakeLists.txt")
include("${parent_dir}/list/src/list_log/CMakeLists.txt")
set(external_include
        ${g_includes}
        ${logger_include}
        ${precision_timer_include}
#        ${list_include}
#        ${list_log_include}
)
set(external_source
        ${sys_utils}
        ${logger_source}
        ${precision_timer_source}
#        ${list_source}
#        ${list_log_source}
)

add_executable(tests
        main.c
        ${mms_source}
        ${mms_loger_source}
        ${external_source}
)
target_include_directories(tests PRIVATE
        ${mms_include}
        ${mms_loger_include}
        ${external_include}
)
