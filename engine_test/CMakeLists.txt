cmake_minimum_required(VERSION 3.28)
project(engine_test C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

# 1. include utility functions
set(cmake_utils "/home/ckalo/data/projects/clion/cmake_utils/proj_utils.cmake")
include(${cmake_utils})
set(root_path ${CLION_ROOT_DIR})
#enable_sanitizer()

# 2. add project's sub-directories
add_subdirectory(scene_sys)
add_subdirectory(layer_sys)
add_subdirectory(ecs_sys)
add_subdirectory(window)
add_subdirectory(renderer)
add_subdirectory(graphics)


# 3. add external projects
include_project(${root_path}/g_includes)
include_project(${root_path}/logger/src)
include_project(${root_path}/file_sys/src)
include_project(${root_path}/string_format/src)
include_project(${root_path}/bits/src)
include_project(${root_path}/memory/src)
include_project(${root_path}/memory/src/mem_log)
include_project(${root_path}/allocators/slab_array/src)
include_project(${root_path}/allocators/slab_array/log)
include_project(${root_path}/allocators/stack/src)
include_project(${root_path}/containers/sparse_array/src)
include_project(${root_path}/containers/sparse_array/log)
include_project(${root_path}/containers/string/src)
include_project(${root_path}/perf_timer/src)
include_project(${root_path}/glfw/src)
include_project(${root_path}/glfw/include/GLFW)
include_project(${root_path}/glfw/deps)
#include_project(/home/ckalo/data/projects/third_party/glad/src)

# HACK: PLATFORM API
# TODO: implement in code as "platform api"

if(UNIX AND NOT APPLE)
    message(STATUS "Building for Linux/Unix")

    # Try to detect preferred platform
    option(GLFW_USE_WAYLAND "Use Wayland backend" OFF)

    if(GLFW_USE_WAYLAND)
        set(GLFW_PLATFORM_WAYLAND TRUE)
        add_definitions(-D_GLFW_WAYLAND)

        # Wayland dependencies
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(WAYLAND REQUIRED wayland-client wayland-egl xkbcommon)

        set(GLFW_LIBRARIES
                ${WAYLAND_LIBRARIES}
                ${CMAKE_DL_LIBS}
        )
    else()
        set(GLFW_PLATFORM_X11 TRUE)
        add_definitions(-D_GLFW_X11)

        # X11 dependencies
        find_package(X11 REQUIRED)

        set(GLFW_LIBRARIES
                ${X11_LIBRARIES}
                Xrandr
                Xinerama
                Xcursor
                Xi
                ${CMAKE_DL_LIBS}
        )
    endif()
else()
    message(STATUS "Building for unknown platform, using NULL backend")
    add_definitions(-D_GLFW_NULL)
endif()


# 4. create executable with all the source files
add_project_executable(engine_test main.c)

# Link math library
target_link_libraries(engine_test m)  # "m" is the linker name for libm

# Debug: global properties
print_all_registered()