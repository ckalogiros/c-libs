#set(mms_logger_include "${CMAKE_CURRENT_LIST_DIR}/")
#set(mms_logger_source
#        "${CMAKE_CURRENT_LIST_DIR}/mem_log.c"
#)

include(/home/ckalo/data/projects/clion/cmake_utils/proj_utils.cmake)
register_module(
        SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/mem_log.c
        HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/mem_log.h
        INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
        #        MODULE=memory
        ENABLE_DEBUG=1
        VERSION_MAJOR=0
)