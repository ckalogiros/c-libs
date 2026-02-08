set(project_dir "${CMAKE_SOURCE_DIR}")
include("${project_dir}/src/CMakeLists.txt")
include("${project_dir}/src/internal/CMakeLists.txt")
include("${project_dir}/src/mem_log/CMakeLists.txt")

################# external directories #################
get_filename_component(parent_dir ${CMAKE_SOURCE_DIR} DIRECTORY)
include("${parent_dir}/g_includes/CMakeLists.txt")
include("${parent_dir}/perf_timer/src/CMakeLists.txt")
include("${parent_dir}/logger/src/CMakeLists.txt")
include("${parent_dir}/logger/src/to_html/CMakeLists.txt")
include("${parent_dir}/allocators/src/slab_allocator/CMakeLists.txt")
include("${parent_dir}/allocators/src/slab_allocator/slab_allocator_log/CMakeLists.txt")
include("${parent_dir}/buffer/src/CMakeLists.txt")
include("${parent_dir}/buffer/src/buffer_logger/CMakeLists.txt")
set(external_include
        ${g_includes}
        ${logger_include}
        ${logger_to_html_include}
        ${precision_timer_include}
        ${slab_allocator_include}
        ${slab_allocator_log_include}
        ${buffer_include}
        ${buffer_log_include}
)
set(external_source
        ${sys_utils}
        ${bitfields}
        ${logger_source}
        ${logger_to_html_source}
        ${precision_timer_source}
        ${slab_allocator_source}
        ${slab_allocator_log_source}
        ${buffer_source}
        ${buffer_log_source}
)

add_executable(tests
        main.c
        tests.c
        ${mms_source}
        ${mms_internal_source}
        ${mms_loger_source}
        ${external_source}
)
target_include_directories(tests PRIVATE
        ${mms_include}
        ${mms_internal_include}
        ${mms_loger_include}
        ${external_include}
)
