
set(project_dir "${CMAKE_SOURCE_DIR}")
include("${project_dir}/src/CMakeLists.txt")
include("${project_dir}/src/buffer_logger/CMakeLists.txt")


################# external directories #################
get_filename_component(parent_dir ${CMAKE_SOURCE_DIR} DIRECTORY)
include("${parent_dir}/g_includes/CMakeLists.txt")
include("${parent_dir}/perf_timer/src/CMakeLists.txt")
include("${parent_dir}/logger/src/CMakeLists.txt")
include("${parent_dir}/memory/src/CMakeLists.txt")
include("${parent_dir}/memory/src/mem_log/CMakeLists.txt")
include("${parent_dir}/allocators/src/slab_allocator/CMakeLists.txt")
include("${parent_dir}/allocators/src/slab_allocator/slab_allocator_log/CMakeLists.txt")
set(external_include
        ${g_includes}
        ${precision_timer_include}
        ${logger_include}
        ${mms_include}
        ${slab_allocator_include}
        ${slab_allocator_log_include}
        ${mms_loger_include}

)
set(external_source
        ${sys_utils}
        ${bitfields}
        ${precision_timer_source}
        ${logger_source}
        ${mms_source}
        ${slab_allocator_source}
        ${slab_allocator_log_source}
        ${mms_loger_source}

)

add_executable(tests main.c
        test_buffer.c
        perf_test_buffer.c
        ${buffer_source}
        ${buffer_log_source}
        ${external_source}
)
target_include_directories(tests PRIVATE
        ${buffer_include}
        ${buffer_log_include}
        ${external_include}
)

