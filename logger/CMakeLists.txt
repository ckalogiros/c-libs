cmake_minimum_required(VERSION 4.1)
project(logger C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

set(project_dir "${CMAKE_SOURCE_DIR}")
include("${project_dir}/src/CMakeLists.txt")


################# external directories #################
get_filename_component(parent_dir ${CMAKE_SOURCE_DIR} DIRECTORY)
include("${parent_dir}/g_includes/CMakeLists.txt")
set(external_include
        ${g_includes}
)
set(external_source
)

add_executable(logger main.c
        ${logger_source}
        ${external_source}
)
target_include_directories(logger PUBLIC
        ${logger_include}
        ${external_include}
)
