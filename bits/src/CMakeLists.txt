cmake_minimum_required(VERSION 3.28.3)

set(CMAKE_C_STANDARD 11)

# 2. add other sub-directories files
register_module(
        sources
        ${CMAKE_CURRENT_LIST_DIR}/bits.c
        ${CMAKE_CURRENT_LIST_DIR}/bitfields.c
        headers
        ${CMAKE_CURRENT_LIST_DIR}/bits.h
        ${CMAKE_CURRENT_LIST_DIR}/bitfields.h
        include_dirs
        ${CMAKE_CURRENT_LIST_DIR}
)
