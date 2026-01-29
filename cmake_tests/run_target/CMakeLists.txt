
#set(project_dir "${CMAKE_SOURCE_DIR}")
#include("${project_dir}/src/CMakeLists.txt")

#target_sources(cmake_tests PRIVATE
#        ${core_sources}
#)
add_executable(tests
        main.c
        ${src_source}
)
target_include_directories(tests PRIVATE
        ${src_include}
)
#message(project_dir="${project_dir}")