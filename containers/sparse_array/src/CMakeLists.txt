include(${cmake_utils})

register_module(
        sources
            ${CMAKE_CURRENT_LIST_DIR}/sparse_arr.c
            ${CMAKE_CURRENT_LIST_DIR}/sparse_arr_internals.c
        headers
            ${CMAKE_CURRENT_LIST_DIR}/sparse_arr.h
            ${CMAKE_CURRENT_LIST_DIR}/sparse_arr_internals.h
            ${CMAKE_CURRENT_LIST_DIR}/sparse_arr_struct.h
            ${CMAKE_CURRENT_LIST_DIR}/designated_initializers.h
        include_dirs
            ${CMAKE_CURRENT_LIST_DIR}
        compiler_macros
            ENABLE_DEBUG=1
            VERSION_MAJOR=0
)
