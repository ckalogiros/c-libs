cmake_minimum_required(VERSION 3.28.3)
project(file_sys C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

# 1. include utility functions
set(cmake_utils "${CMAKE_CURRENT_LIST_DIR}/../cmake_utils/proj_utils.cmake")
include(${cmake_utils})
set(root_path ${ROOT_DIR})
#enable_sanitizer()

# 2. add /src that adds .c.h files and adds other sub-directories
add_subdirectory(src)
#add_subdirectory(test)

include_project(${root_path}/g_includes)
include_project(${root_path}/logger/src)
include_project(${root_path}/string_format/src)
include_project(${root_path}/bits/src)
include_project(${root_path}/memory/src)
include_project(${root_path}/memory/src/mem_log)
include_project(${root_path}/allocators/slab_array/src)
include_project(${root_path}/allocators/slab_array/log)
include_project(${root_path}/allocators/stack/src)
include_project(${root_path}/allocators/queue/src)
include_project(${root_path}/containers/sparse_array/src)
include_project(${root_path}/containers/sparse_array/log)
include_project(${root_path}/containers/string/src)
include_project(${root_path}/perf_timer/src)

# 3. create executable with all the source files
add_project_executable(file_sys main.c)

# Debug: global properties
print_all_registered()