include(/home/ckalo/data/projects/clion/cmake_utils/proj_utils.cmake)

register_module(
        SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/source111.c
        HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/header111.h
        INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
        DEFINITIONS
        MODULE_SPECIAL=1
)
