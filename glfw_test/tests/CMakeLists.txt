
set(project_dir "${CMAKE_SOURCE_DIR}")
include("${project_dir}/src/CMakeLists.txt")
include("${project_dir}/tests/triangle/CMakeLists.txt")
include("${project_dir}/tests/line/CMakeLists.txt")
include("${project_dir}/tests/instanced/CMakeLists.txt")
include("${project_dir}/tests/geometry_shader/CMakeLists.txt")



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
include("/home/ckalo/data/projects/third_party/glad/src/CMakeLists.txt")
set(external_include
        ${g_includes}
        ${precision_timer_include}
        ${logger_include}
        ${mms_include}
        ${slab_allocator_include}
        ${slab_allocator_log_include}
        ${mms_loger_include}
        ${glad_include}
        ${buffer_include}
        ${buffer_log_include}
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
        ${buffer_source}
        ${buffer_log_source}
        ${glad_source}
)


set(test_renders_include
        ${triangle_include}
        ${line_include}
        ${instanced_include}
        ${geometry_shader_include}
)
set(test_renders_source
        gl_perf_timer.c
        ${triangle_source}
        ${line_source}
        ${instanced_source}
        ${geometry_shader_source}
)

add_executable(tests main.c
        ${test_renders_source}
        ${external_source}
)
target_include_directories(tests PRIVATE
        "${project_dir}/tests"
        ${test_renders_include}
        ${external_include}
        "/home/ckalo/data/projects/third_party/glfw-source/include/GLFW"
)

# Link static library
target_link_libraries(tests
#        "/home/ckalo/data/projects/third_party/glfw-source/cmake-build-minsizerel/src/libglfw3.a"
        "/home/ckalo/data/projects/third_party/glfw-source/cmake-build-debug/src/libglfw3.a"
        X11 pthread dl m     # GLFW dependencies
        ${OPENGL_LIBRARIES}  # OpenGL
)

