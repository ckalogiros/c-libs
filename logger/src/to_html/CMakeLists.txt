include(${cmake_utils})

register_module(
        sources
        ${CMAKE_CURRENT_LIST_DIR}/logger_to_html.c
        headers
        ${CMAKE_CURRENT_LIST_DIR}/logger_to_html.h
        ${CMAKE_CURRENT_LIST_DIR}/logger_to_html_css.h
        include_dirs
        ${CMAKE_CURRENT_LIST_DIR}
        compiler_macros
        ENABLE_DEBUG=1
        VERSION_MAJOR=0
)