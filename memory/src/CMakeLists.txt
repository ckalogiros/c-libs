include(${cmake_utils})

register_module(
        sources
            ${CMAKE_CURRENT_LIST_DIR}/mem_man.c
            ${CMAKE_CURRENT_LIST_DIR}/mem_man_internals.c
        headers
            ${CMAKE_CURRENT_LIST_DIR}/mem_man_interface.h
            ${CMAKE_CURRENT_LIST_DIR}/mem_man_internals.h
        include_dirs
            ${CMAKE_CURRENT_LIST_DIR}
        compiler_macros
            ENABLE_DEBUG=1
            VERSION_MAJOR=0
)
