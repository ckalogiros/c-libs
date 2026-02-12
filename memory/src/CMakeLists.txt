#set(mms_include "${CMAKE_CURRENT_LIST_DIR}/")
#set(mms_source
#        "${CMAKE_CURRENT_LIST_DIR}/mem_man.c"
#        "${CMAKE_CURRENT_LIST_DIR}/mem_man_internals.c"
#)



include(/home/ckalo/data/projects/clion/cmake_utils/proj_utils.cmake)

add_subdirectory(internal)
add_subdirectory(mem_log)

register_module(
        SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/mem_man.c
        ${CMAKE_CURRENT_SOURCE_DIR}/mem_man_internals.c
        HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/mem_man_interface.h
        ${CMAKE_CURRENT_SOURCE_DIR}/mem_man_internals.h
        INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
        #        MODULE=memory
        ENABLE_DEBUG=1
        VERSION_MAJOR=0
)

