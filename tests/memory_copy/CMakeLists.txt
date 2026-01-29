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

add_executable(mem_copy mem_copy_main.c
#        ecs.c
        ${external_source}
)
target_include_directories(mem_copy PRIVATE
        ${external_include}
)

# Link the math library (libm) to the "math_app" executable
#target_link_libraries(tests m)  # "m" is the linker name for libm
