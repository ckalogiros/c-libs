include(${parent_dir}/cmake_utils/proj_utils.cmake)

register_module(
        sources
            ${CMAKE_CURRENT_SOURCE_DIR}/source11.c
        headers
            ${CMAKE_CURRENT_SOURCE_DIR}/header11.h
        include_dirs
            ${CMAKE_CURRENT_SOURCE_DIR}
        compiler_macros
#            MODULE=0
)

add_subdirectory(dir1)