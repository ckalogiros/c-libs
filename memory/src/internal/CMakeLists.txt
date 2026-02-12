#set(mms_internal_include "${CMAKE_CURRENT_LIST_DIR}/")
#set(mms_internal_source
##        "${CMAKE_CURRENT_LIST_DIR}/mem_test.c"
#)


include(/home/ckalo/data/projects/clion/cmake_utils/proj_utils.cmake)
register_module(
        SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/source11.c
        HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/mem_structs.h
        INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
        DEFINITIONS
#        MODULE=memory-internal
        ENABLE_DEBUG=1
        VERSION_MAJOR=0
)