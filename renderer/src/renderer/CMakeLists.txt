cmake_minimum_required(VERSION 3.28.3)

set(CMAKE_C_STANDARD 11)

# 2. add other sub-directories files
register_module(
        sources
        ${CMAKE_CURRENT_LIST_DIR}/renderer.c
        headers
        ${CMAKE_CURRENT_LIST_DIR}/renderer.h
        ${CMAKE_CURRENT_LIST_DIR}/renderer_structs.h
        ${CMAKE_CURRENT_LIST_DIR}/renderer_internals.h
        include_dirs
        ${CMAKE_CURRENT_LIST_DIR}
)
