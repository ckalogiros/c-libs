cmake_minimum_required(VERSION 4.1)
project(cmake_tests C)

set(CMAKE_C_STANDARD 11)


set(project_dir "${CMAKE_SOURCE_DIR}")
include("${project_dir}/src/CMakeLists.txt")
add_subdirectory(${project_dir}/run_target)

################# external directories #################
get_filename_component(parent_dir ${CMAKE_SOURCE_DIR} DIRECTORY)
include("${parent_dir}/cmake_tests_external_files/src/CMakeLists.txt")
include("${parent_dir}/g_includes/CMakeLists.txt")
set(external_include
        ${src_ext_include}
        ${g_includes}
)
set(external_source
        ${src_ext_source}
)


add_executable(cmake_tests main.c
        ${src_source}
        ${external_source}
)
target_include_directories(cmake_tests PRIVATE
        ${src_include}
        ${external_include}
)


message(parent_dir="${parent_dir}")
