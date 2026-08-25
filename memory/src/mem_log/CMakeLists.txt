include(${cmake_utils})

register_module(
        sources
            ${CMAKE_CURRENT_LIST_DIR}/mem_log.c
            ${CMAKE_CURRENT_LIST_DIR}/mem_fprint_debug.c
        headers
            ${CMAKE_CURRENT_LIST_DIR}/mem_log.h
            ${CMAKE_CURRENT_LIST_DIR}/mem_fprint_debug.h
        include_dirs
            ${CMAKE_CURRENT_LIST_DIR}
        compiler_macros
            ENABLE_DEBUG=1
            VERSION_MAJOR=0
)