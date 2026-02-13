include(${cmake_utils})

register_module(
        sources
            ${CMAKE_CURRENT_LIST_DIR}/logger.c
        headers
            ${CMAKE_CURRENT_LIST_DIR}/logger.h
            ${CMAKE_CURRENT_LIST_DIR}/logger_color.h
        include_dirs
            ${CMAKE_CURRENT_LIST_DIR}
        compiler_macros
            ENABLE_DEBUG=1
            VERSION_MAJOR=0
)