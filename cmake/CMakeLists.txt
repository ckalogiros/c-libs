cmake_minimum_required(VERSION 3.28.3)
project(cmake C)

set(CMAKE_C_STANDARD 11)

include(/home/ckalo/data/projects/clion/cmake_utils/proj_utils.cmake)

add_subdirectory(src)

get_property(ALL_SOURCES GLOBAL PROPERTY GLOBAL_SOURCES)
message("ALL_SOURCES before add_executable: ${ALL_SOURCES}")
add_executable(cmake main.c ${ALL_SOURCES})
#get_target_property(INCLUDES1 cmake INCLUDE_DIRECTORIES)
#message("After add_executable: ${INCLUDES1}")

#target_include_directories(cmake PRIVATE ${ALL_INCLUDE_DIRS})

# Direct property setting instead of target_include_directories
set_property(TARGET cmake PROPERTY INCLUDE_DIRECTORIES ${ALL_INCLUDE_DIRS})
get_target_property(INCLUDES2 cmake INCLUDE_DIRECTORIES)
message("After set_property: ${INCLUDES2}")

target_include_directories(cmake PUBLIC
        /home/ckalo/data/projects/clion/cmake/src
        /home/ckalo/data/projects/clion/cmake/src/dir1
)

# At the VERY END
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

get_property(ALL_INCLUDE_DIRS GLOBAL PROPERTY GLOBAL_INCLUDE_DIRS)
message("All include dirs: ${ALL_INCLUDE_DIRS}")

# Force write a file with the include paths
file(WRITE ${CMAKE_BINARY_DIR}/include_dirs.txt
        "Include directories for cmake target:\n${ALL_INCLUDE_DIRS}")

#get_target_property(INCLUDES cmake INCLUDE_DIRECTORIES)
#message("FINAL - Target includes: ${INCLUDES}")

# Verify
#get_target_property(INCLUDES cmake INCLUDE_DIRECTORIES)
#message("INCLUDE_DIRECTORIES property: ${INCLUDES}")



#print_target_includes(cmake)
#
### Debug: global properties
#print_all_registered()