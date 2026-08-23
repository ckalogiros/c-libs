cmake_minimum_required(VERSION 3.28)
project(object_test C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

# 1. include utility functions
set(cmake_utils "${CMAKE_CURRENT_LIST_DIR}/../../cmake_utils/proj_utils.cmake")
include(${cmake_utils})
set(root_path ${CLION_ROOT_DIR})
#enable_sanitizer()

# 2. add project's sub-directories
add_subdirectory(src)
add_subdirectory(tests)
include(${cmake_utils})


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



# 4. create executable with all the source files
add_project_executable(object_test main.c)

# Link math library
target_link_libraries(object_test m)  # "m" is the linker name for libm

# Debug: global properties
print_all_registered()