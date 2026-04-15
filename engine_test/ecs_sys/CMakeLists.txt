include(${cmake_utils})
register_module(
        sources
        ${CMAKE_CURRENT_LIST_DIR}/entity.c
#        ${CMAKE_CURRENT_LIST_DIR}/groups.c
#        ${CMAKE_CURRENT_LIST_DIR}/ecs.c
        ${CMAKE_CURRENT_LIST_DIR}/component.c
#        ${CMAKE_CURRENT_LIST_DIR}/tests.c
        headers
        ${CMAKE_CURRENT_LIST_DIR}/entity.h
#        ${CMAKE_CURRENT_LIST_DIR}/groups.h
#        ${CMAKE_CURRENT_LIST_DIR}/ecs.h
        ${CMAKE_CURRENT_LIST_DIR}/component.h
#        ${CMAKE_CURRENT_LIST_DIR}/tests.h
        include_dirs
        ${CMAKE_CURRENT_LIST_DIR}
        compiler_macros
        ENABLE_DEBUG=1
        VERSION_MAJOR=0
)