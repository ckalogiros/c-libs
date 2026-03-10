include(${cmake_utils})

register_module(
        sources
        ${CMAKE_CURRENT_LIST_DIR}/stringc.c
        ${CMAKE_CURRENT_LIST_DIR}/string_internals.c
        headers
        ${CMAKE_CURRENT_LIST_DIR}/stringc.h
        ${CMAKE_CURRENT_LIST_DIR}/string_internals.h
        ${CMAKE_CURRENT_LIST_DIR}/string_struct.h
        include_dirs
        ${CMAKE_CURRENT_LIST_DIR}
        compiler_macros
        ENABLE_DEBUG=1
        VERSION_MAJOR=0
)
