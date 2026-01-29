cmake_minimum_required(VERSION 4.1)
project(cmake_tests_external_files C)

set(CMAKE_C_STANDARD 11)

set(project_dir "${CMAKE_SOURCE_DIR}")
include("${project_dir}/src/CMakeLists.txt")
message(CMakeLists="${project_dir}/src/CMakeLists.txt")

add_executable(cmake_tests_external_files main.c
    ${src_ext_source}
)

target_include_directories(cmake_tests_external_files PRIVATE ${src_ext_include})
