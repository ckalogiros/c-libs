include(${cmake_utils})

register_module(
        sources
        ${CMAKE_CURRENT_LIST_DIR}/precision_timer.c
        ${CMAKE_CURRENT_LIST_DIR}/precision_timer_logger.c
        ${CMAKE_CURRENT_LIST_DIR}/sparse_arr_impl.c
        ${CMAKE_CURRENT_LIST_DIR}/cpu_time_stamp.c
        headers
        ${CMAKE_CURRENT_LIST_DIR}/precision_timer.h
        ${CMAKE_CURRENT_LIST_DIR}/precision_timer_struct.h
        ${CMAKE_CURRENT_LIST_DIR}/precision_timer_logger.h
        ${CMAKE_CURRENT_LIST_DIR}/sparse_arr_impl.h
        ${CMAKE_CURRENT_LIST_DIR}/cpu_time_stamp.h
        include_dirs
        ${CMAKE_CURRENT_LIST_DIR}
        compiler_macros
        ENABLE_DEBUG=1
        VERSION_MAJOR=0
)

