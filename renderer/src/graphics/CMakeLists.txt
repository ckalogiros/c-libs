cmake_minimum_required(VERSION 3.28.3)

set(CMAKE_C_STANDARD 11)

# 2. add other sub-directories files
register_module(
        sources
        ${CMAKE_CURRENT_LIST_DIR}/opengl.c
        ${CMAKE_CURRENT_LIST_DIR}/shader_build.c
        headers
        ${CMAKE_CURRENT_LIST_DIR}/opengl.h
        ${CMAKE_CURRENT_LIST_DIR}/shader_build.h
        include_dirs
        ${CMAKE_CURRENT_LIST_DIR}
)
