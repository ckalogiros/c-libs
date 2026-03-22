# Function to register a module
function(register_module)

    set(options)
    set(oneValueArgs)
    set(multiValueArgs sources headers include_dirs compiler_macros)

    cmake_parse_arguments(register "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Append sources
    get_property(all_source_files GLOBAL PROPERTY exernal_source_files)
    set_property(GLOBAL PROPERTY exernal_source_files ${all_source_files} ${register_sources})

    # Append headers
    get_property(all_header_files GLOBAL PROPERTY external_header_files)
    set_property(GLOBAL PROPERTY external_header_files ${all_header_files} ${register_headers})

    # Append include directories
    get_property(all_include_dirs GLOBAL PROPERTY external_incude_dirs)
    set_property(GLOBAL PROPERTY external_incude_dirs ${all_include_dirs} ${register_include_dirs})

    # Append compiler_macros
    get_property(CURRENT_DEFINITIONS GLOBAL PROPERTY external_compiler_macros)
    set_property(GLOBAL PROPERTY external_compiler_macros ${CURRENT_DEFINITIONS} ${register_compiler_macros})
endfunction()

# Function to create executable with all dependencies
function(add_project_executable TARGET_NAME)
    get_property(all_source_files GLOBAL PROPERTY exernal_source_files)
    get_property(all_include_dirs GLOBAL PROPERTY external_incude_dirs)
    get_property(all_compiler_macros GLOBAL PROPERTY external_compiler_macros)

    add_executable(${TARGET_NAME} ${all_source_files} ${ARGN})
    target_include_directories(${TARGET_NAME} PRIVATE ${all_include_dirs})
    target_compile_definitions(${TARGET_NAME} PRIVATE ${all_compiler_macros})
endfunction()

# Include another project's source registration
function(include_project PROJECT_PATH)
    # Construct path to the project's src/CMakeLists.txt
    set(SRC_CMAKE "${PROJECT_PATH}/CMakeLists.txt")

    if(EXISTS ${SRC_CMAKE})
        message("=== Including sub directory: ${PROJECT_PATH} ===")
        include(${SRC_CMAKE})
    else()
        message(FATAL_ERROR "Cannot find ${SRC_CMAKE}")
    endif()
endfunction()

########################################################################################
# Enable AddressSanitizer for debugging
function(enable_sanitizer)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        # Debug mode - only ASAN, explicitly disable LSAN
        message(STATUS "Debug: ASAN enabled, LSAN disabled")

        add_compile_options(-fsanitize=address -g -O0 -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address)

        # Disable LSAN completely
        set(ENV{ASAN_OPTIONS} "detect_leaks=0")
        set(ENV{LSAN_OPTIONS} "detect_leaks=0")

        # Also set as compile definition
        add_compile_definitions(ASAN_OPTIONS="detect_leaks=0")

    else()
        # Non-debug builds - full sanitizer
        message(STATUS "Sanitize build: ASAN+LSAN enabled")

        add_compile_options(-fsanitize=address,leak -g -O0 -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address,leak)

        set(ENV{ASAN_OPTIONS} "detect_leaks=1")
        set(ENV{LSAN_OPTIONS} "verbosity=1:log_threads=1:detect_leaks=1")
    endif()
endfunction()
#function(enable_sanitizer)
#
#    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address -g")
#    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")
#
#    # Set LSAN options for all tests
#    set(ENV{LSAN_OPTIONS} "verbosity=1:log_threads=1")
#
#    # Or more generally for all builds (but typically just for Debug)
#    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#        add_compile_options(-fsanitize=address -g)
#        add_link_options(-fsanitize=address)
#
#        # Disable leak detection when running under debugger
#        set(ENV{ASAN_OPTIONS} "detect_leaks=0")
#        add_compile_definitions(ASAN_OPTIONS="detect_leaks=0")
#    endif()
#endfunction()
########################################################################################


#
## In your proj_utils.cmake, modify include_project:
#function(include_project PROJECT_PATH)
#    # Create a unique variable name based on the project path
#    # Replace slashes with underscores to create a valid CMake variable name
#    string(REPLACE "/" "_" PROJECT_ID "${PROJECT_PATH}")
#    string(REPLACE "-" "_" PROJECT_ID "${PROJECT_ID}")
#    string(REPLACE "." "_" PROJECT_ID "${PROJECT_ID}")
#
#    # Check if this project has already been included
#    get_property(INCLUDED_PROJECTS GLOBAL PROPERTY included_projects_list)
#
#    # Alternative: Check a cache variable specific to this project
#    if(NOT DEFINED CACHE{${PROJECT_ID}_INCLUDED})
#        message("=== Including sub directory: ${PROJECT_PATH} ===")
#
#        # Mark as included in cache (persists across directories)
#        set(${PROJECT_ID}_INCLUDED TRUE CACHE INTERNAL "Track if ${PROJECT_PATH} is included")
#
#        # Also add to global property list for debugging
#        set_property(GLOBAL APPEND PROPERTY included_projects_list "${PROJECT_PATH}")
#
#        # Include the project's CMakeLists.txt
#        set(SRC_CMAKE "${PROJECT_PATH}/CMakeLists.txt")
#        if(EXISTS ${SRC_CMAKE})
#            include(${SRC_CMAKE})
#        else()
#            message(FATAL_ERROR "Cannot find ${SRC_CMAKE}")
#        endif()
#    else()
#        message(STATUS "=== Skipping already included: ${PROJECT_PATH} ===")
#    endif()
#endfunction()
#
## Optional: Function to clear all tracking (useful for clean rebuilds)
#function(reset_project_tracking)
#    get_property(INCLUDED_PROJECTS GLOBAL PROPERTY included_projects_list)
#    foreach(PROJECT ${INCLUDED_PROJECTS})
#        string(REPLACE "/" "_" PROJECT_ID "${PROJECT}")
#        string(REPLACE "-" "_" PROJECT_ID "${PROJECT_ID}")
#        string(REPLACE "." "_" PROJECT_ID "${PROJECT_ID}")
#        unset(${PROJECT_ID}_INCLUDED CACHE)
#    endforeach()
#    set_property(GLOBAL PROPERTY included_projects_list "")
#    message(STATUS "=== Project tracking reset ===")
#endfunction()
















###########################################################################
# debug file directories
# Print all registered sources
function(print_sources)
    get_property(all_source_files GLOBAL PROPERTY exernal_source_files)
    message("=== Source Files ===")
    foreach(source IN LISTS all_source_files)
        message("  ${source}")
    endforeach()
endfunction()

# Print all registered headers
function(print_headers)
    get_property(hfiles GLOBAL PROPERTY external_header_files)
    message("=== Header Files ===")
    foreach(header IN LISTS hfiles)
        message("  ${header}")
    endforeach()
endfunction()

# Print all registered include directories
function(print_includes)
    get_property(ALL_INCLUDE_DIRS GLOBAL PROPERTY external_incude_dirs)
    message("=== Include Directories ===")
    foreach(inc_dir IN LISTS ALL_INCLUDE_DIRS)
        message("  ${inc_dir}")
    endforeach()
endfunction()

# Print all registered compiler_macros
function(print_compiler_macros)
    get_property(all_compiler_macros GLOBAL PROPERTY external_compiler_macros)
    message("=== Definitions ===")
    foreach(def IN LISTS all_compiler_macros)
        message("  ${def}")
    endforeach()
endfunction()

# Print everything at once
function(print_all_registered)
    print_sources()
    print_headers() 
    print_includes()
#    print_compiler_macros()
endfunction()


# another debug
function(print_target_includes TARGET_NAME)
    get_target_property(INCLUDES ${TARGET_NAME} INCLUDE_DIRECTORIES)
    message("=== Target ${TARGET_NAME} Include Directories ===")
    if(INCLUDES)
        foreach(inc IN LISTS INCLUDES)
            message("  ${inc}")
        endforeach()
    else()
        message("  (none)")
    endif()
endfunction()
