include(${cmake_utils})

register_module(
        sources
            ${CMAKE_CURRENT_LIST_DIR}/mbuffer.c
        headers
            ${CMAKE_CURRENT_LIST_DIR}/mbuffer_interface.h
            ${CMAKE_CURRENT_LIST_DIR}/mbuffer.h
        include_dirs
            ${CMAKE_CURRENT_LIST_DIR}
        compiler_macros
            ENABLE_DEBUG=1
            VERSION_MAJOR=0
)
