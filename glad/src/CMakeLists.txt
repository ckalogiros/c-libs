include(${cmake_utils})

register_module(
        sources
                ${CMAKE_CURRENT_LIST_DIR}/glad.c
        headers
                ${CMAKE_CURRENT_LIST_DIR}/glad.h
                ${CMAKE_CURRENT_LIST_DIR}/khrplatform.h
        include_dirs
                ${CMAKE_CURRENT_LIST_DIR}
        compiler_macros
            ENABLE_DEBUG=1
            VERSION_MAJOR=0
)
