include(${cmake_utils})

register_module(
        sources
        ${CMAKE_CURRENT_LIST_DIR}/buffer.c
        ${CMAKE_CURRENT_LIST_DIR}/buffer_internals.c
        headers
        ${CMAKE_CURRENT_LIST_DIR}/buffer.h
        ${CMAKE_CURRENT_LIST_DIR}/buffer_internals.h
        ${CMAKE_CURRENT_LIST_DIR}/buffer_struct.h
        ${CMAKE_CURRENT_LIST_DIR}/designated_initializers.h
        include_dirs
        ${CMAKE_CURRENT_LIST_DIR}
        compiler_macros
            ENABLE_DEBUG=1
            VERSION_MAJOR=0
)
