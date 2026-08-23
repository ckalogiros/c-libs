cmake_minimum_required(VERSION 3.28)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

# 1. include utility functions
set(cmake_utils "${CMAKE_CURRENT_LIST_DIR}/../../cmake_utils/proj_utils.cmake")
#set(cmake_utils "/home/ckalo/data/projects/clion/cmake_utils/proj_utils.cmake")
include(${cmake_utils})
set(root_path ${CLION_ROOT_DIR})


#enable_sanitizer()

# 2. add project's sub-directories
#add_subdirectory(src)
#add_subdirectory(tests)


register_module(
        sources
        ${CMAKE_CURRENT_LIST_DIR}/entity.c
        ${CMAKE_CURRENT_LIST_DIR}/groups.c
        ${CMAKE_CURRENT_LIST_DIR}/ecs.c
        ${CMAKE_CURRENT_LIST_DIR}/component.c
        ${CMAKE_CURRENT_LIST_DIR}/tests.c
        headers
        ${CMAKE_CURRENT_LIST_DIR}/entity.h
        ${CMAKE_CURRENT_LIST_DIR}/groups.h
        ${CMAKE_CURRENT_LIST_DIR}/ecs.h
        ${CMAKE_CURRENT_LIST_DIR}/component.h
        ${CMAKE_CURRENT_LIST_DIR}/tests.h
        include_dirs
        ${CMAKE_CURRENT_LIST_DIR}
        compiler_macros
        ENABLE_DEBUG=1
        VERSION_MAJOR=0
)

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
include_project(${root_path}/allocators/queue/src)
include_project(${root_path}/allocators/arena/src)
include_project(${root_path}/containers/buffer_array/src)
include_project(${root_path}/containers/buffer_array/log)
include_project(${root_path}/containers/string/src)
include_project(${root_path}/perf_timer/src)

# 3. create executable with all the source files
add_project_executable(ecs ecs_main.c)

# Link the math library (libm) to the "math_app" executable
target_link_libraries(ecs m)  # "m" is the linker name for libm

# Debug: global properties
print_all_registered()




#set(CMAKE_C_STANDARD 11)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
#
## 1. include utility functions
#set(cmake_utils "${CMAKE_CURRENT_LIST_DIR}/../../cmake_utils/proj_utils.cmake")
#include(${cmake_utils})
#set(root_path ${CLION_ROOT_DIR})
##enable_sanitizer()
#
## 2. add project's sub-directories
#add_subdirectory(src)
#add_subdirectory(tests)
#include(${cmake_utils})
#
#
## 3. add external projects
#include_project(${root_path}/g_includes)
#include_project(${root_path}/logger/src)
#include_project(${root_path}/file_sys/src)
#include_project(${root_path}/string_format/src)
#include_project(${root_path}/bits/src)
#include_project(${root_path}/memory/src)
#include_project(${root_path}/memory/src/mem_log)
#include_project(${root_path}/allocators/slab_array/src)
#include_project(${root_path}/allocators/slab_array/log)
#include_project(${root_path}/allocators/stack/src)
#include_project(${root_path}/allocators/queue/src)
#include_project(${root_path}/allocators/arena/src)
#include_project(${root_path}/containers/buffer_array/src)
#include_project(${root_path}/containers/buffer_array/log)
#include_project(${root_path}/containers/string/src)
#include_project(${root_path}/perf_timer/src)
