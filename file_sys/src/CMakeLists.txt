include(${cmake_utils})

#if(NOT G_INCLUDES_ADDED)
    register_module(
            sources
            ${CMAKE_CURRENT_LIST_DIR}/file_sys.c
            headers
            ${CMAKE_CURRENT_LIST_DIR}/file_sys_interface.h
            include_dirs
            ${CMAKE_CURRENT_LIST_DIR}
            compiler_macros
            ENABLE_DEBUG=1
            VERSION_MAJOR=0
    )
#    set(G_INCLUDES_ADDED TRUE CACHE INTERNAL "G includes added")
#endif()
