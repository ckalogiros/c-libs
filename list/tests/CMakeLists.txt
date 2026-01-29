
set(project_dir "${CMAKE_SOURCE_DIR}")
include("${project_dir}/src/CMakeLists.txt")
include("${project_dir}/src/list_log/CMakeLists.txt")


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

add_executable(tests main.c
        tests.c
        ${list_source}
        ${list_log_source}
        ${external_source}
)
target_include_directories(tests PRIVATE
        ${list_include}
        ${list_log_include}
        ${external_include}
)

