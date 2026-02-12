

include(/home/ckalo/data/projects/clion/cmake_utils/proj_utils.cmake)

#add_subdirectory(subdir1)
#add_subdirectory(subdir2)

message("=== Registering module from ${CMAKE_CURRENT_SOURCE_DIR} ===")

register_module(
        SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/source11.c
        HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/header11.h
        INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
        DEFINITIONS
        MODULE_SPECIAL=1
)


add_subdirectory(dir1)