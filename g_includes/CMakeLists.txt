cmake_minimum_required(VERSION 3.28.3)
#project(g_includes C)

set(CMAKE_C_STANDARD 11)

get_filename_component(parent_dir ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)
include(${parent_dir}/cmake_utils/proj_utils.cmake)

register_module(
        sources
            ${CMAKE_CURRENT_LIST_DIR}/sys_utils.c
            ${CMAKE_CURRENT_LIST_DIR}/str_utils.c
        headers
            ${CMAKE_CURRENT_LIST_DIR}/sys_utils.h
            ${CMAKE_CURRENT_LIST_DIR}/str_utils.h
            ${CMAKE_CURRENT_LIST_DIR}/my_types.h
            ${CMAKE_CURRENT_LIST_DIR}/my_macros.h
            ${CMAKE_CURRENT_LIST_DIR}/smath.h
            ${CMAKE_CURRENT_LIST_DIR}/alignment.h
        include_dirs
            ${CMAKE_CURRENT_LIST_DIR}
        compiler_macros
            ENABLE_DEBUG=1
            VERSION_MAJOR=0
)

#print_all_registered()