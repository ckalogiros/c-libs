include(${cmake_utils})

register_module(
        sources
            ${CMAKE_CURRENT_LIST_DIR}/file_sys.c
            ${CMAKE_CURRENT_LIST_DIR}/file_sys_debug.c
        headers
            ${CMAKE_CURRENT_LIST_DIR}/file_sys_interface.h
            ${CMAKE_CURRENT_LIST_DIR}/file_sys_debug.h
        include_dirs
            ${CMAKE_CURRENT_LIST_DIR}
        compiler_macros
            ENABLE_DEBUG=1
            VERSION_MAJOR=0
)