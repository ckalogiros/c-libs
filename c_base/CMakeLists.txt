cmake_minimum_required(VERSION 4.1)
project(c_base C)

set(CMAKE_C_STANDARD 11)

################# cash directories #################
get_filename_component(parent_dir ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
set(project_dir "${CMAKE_SOURCE_DIR}")
set(path "${parent_dir}")


################# external source files #################
set(external_perf_timers  "${path}/perf_timer/src")
include("${external_perf_timers}/CMakeLists.txt")

include_directories(
        PUBLIC "${precision_timer_include}"
)

set(all_ext_source_files
        ${precision_timer_source}
)

#add_executable(c_base main.c)
add_executable (
        c_base main.c
        ${all_ext_source_files}
)

function(print var)
    message(STATUS "${var} = ${${var}}")
endfunction()

print("--- path: ${path} ---")
print("--- project_dir: ${project_dir} ---")
print("--- outer dir: ${path}/perf_timer ---")
print("--- precision_timer_source: ${precision_timer_source} ---")
print("--- precision_timer_include: ${precision_timer_include} ---")




