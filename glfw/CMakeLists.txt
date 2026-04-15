cmake_minimum_required(VERSION 3.28)
project(glfw C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

# 1. include utility functions
set(cmake_utils "/home/ckalo/data/projects/clion/cmake_utils/proj_utils.cmake")
include(${cmake_utils})
set(root_path ${CLION_ROOT_DIR})
enable_sanitizer()

# 2. add project's sub-directories
add_subdirectory(include/GLFW)
add_subdirectory(src)
add_subdirectory(deps)
add_subdirectory(tests)

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
#include_project(/home/ckalo/data/projects/third_party/glfw)
#include_project(/home/ckalo/data/projects/third_party/GLFW-CMake-starter)
#include_project(/home/ckalo/data/projects/third_party/glad/src)

#add_subdirectory(../glfw ${CMAKE_BINARY_DIR}/glfw-build)
#add_subdirectory(/home/ckalo/data/projects/third_party/glfw ${CMAKE_BINARY_DIR}/glfw-build)
#add_subdirectory(/home/ckalo/data/projects/third_party/glfw)

## Override GLFW_BUILD_* variables BEFORE adding subdirectory
#if (WIN32)
#    set(GLFW_BUILD_WIN32 ON)
#    set(GLFW_BUILD_COCOA OFF)
#    set(GLFW_BUILD_X11 OFF)
#    set(GLFW_BUILD_WAYLAND OFF)
#elseif (APPLE)
#    set(GLFW_BUILD_WIN32 OFF)
#    set(GLFW_BUILD_COCOA ON)
#    set(GLFW_BUILD_X11 OFF)
#    set(GLFW_BUILD_WAYLAND OFF)
#else()  # Linux/Unix
#    set(GLFW_BUILD_WIN32 OFF)
#    set(GLFW_BUILD_COCOA OFF)
#    set(GLFW_BUILD_X11 ON)     # Build X11 support
#    set(GLFW_BUILD_WAYLAND OFF) # Or ON for Wayland, or both
#endif()

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
add_project_executable(glfw main.c)

# Link math library
target_link_libraries(glfw PRIVATE m)

# Debug: global properties
print_all_registered()



## The time, thread and module code is shared between all backends on a given OS,
## including the null backend, which still needs those bits to be functional
#if (APPLE)
#    target_sources(glfw PRIVATE cocoa_time.h cocoa_time.c posix_thread.h
#            posix_module.c posix_thread.c)
#elseif (WIN32)
#    target_sources(glfw PRIVATE win32_time.h win32_thread.h win32_module.c
#            win32_time.c win32_thread.c)
#else()
#    target_sources(glfw PRIVATE posix_time.h posix_thread.h posix_module.c
#            posix_time.c posix_thread.c)
#endif()
