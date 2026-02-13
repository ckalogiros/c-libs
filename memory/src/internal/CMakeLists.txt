include(${cmake_utils})

register_module(
        sources

        headers
            ${CMAKE_CURRENT_LIST_DIR}/mem_structs.h
        include_dirs
            ${CMAKE_CURRENT_LIST_DIR}
        compiler_macros
            ENABLE_DEBUG=1
            VERSION_MAJOR=0
)