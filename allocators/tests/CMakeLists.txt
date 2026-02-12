set(project_dir "${CMAKE_SOURCE_DIR}")
include("${project_dir}/src/slab_allocator/CMakeLists.txt")
include("${project_dir}/src/slab_allocator/slab_allocator_log/CMakeLists.txt")
include("${project_dir}/src/stack_allocator/CMakeLists.txt")
include("${project_dir}/src/generic_allocator/CMakeLists.txt")


################# external directories #################
get_filename_component(parent_dir ${CMAKE_SOURCE_DIR} DIRECTORY)
include("${parent_dir}/g_includes/CMakeLists.txt")

#include("${parent_dir}/memory/src/CMakeLists.txt")
#include("${parent_dir}/memory/src/internal/CMakeLists.txt")
#include("${parent_dir}/memory/src/mem_log/CMakeLists.txt")
include("${parent_dir}/memory/CMakeLists.txt")

#include("${parent_dir}/perf_timer/src/CMakeLists.txt")
include("${parent_dir}/logger/src/CMakeLists.txt")
#include("${parent_dir}/buffer/src/CMakeLists.txt")
#include("${parent_dir}/buffer/src/buffer_logger/CMakeLists.txt")
#include("${parent_dir}/io/src/std_input/CMakeLists.txt")

set(external_include
        ${g_includes}
        ${logger_include}
        ${mms_source}
        ${mms_internal_source}
        ${mms_logger_source}
#        ${precision_timer_include}
#        ${buffer_include}
#        ${buffer_log_include}
##        ${std_input_include}
)
set(external_source
        ${sys_utils}
        ${bitfields}
        ${logger_source}
        ${mms_source}
        ${mms_internal_source}
        ${mms_logger_source}
#        ${precision_timer_source}
#        ${buffer_source}
#        ${buffer_log_source}
##        ${std_input_source}
)

add_executable(tests main.c
        test_performance.c
        ${external_source}
        ${allocators_source}
        ${slab_allocator_source}
        ${slab_allocator_log_source}
)
target_include_directories(tests PRIVATE
        ${external_include}
        ${allocators_include}
        ${slab_allocator_include}
        ${slab_allocator_log_include}
)

