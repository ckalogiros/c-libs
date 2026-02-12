

# Function to register a module
function(register_module)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs SOURCES HEADERS INCLUDE_DIRS DEFINITIONS)

    cmake_parse_arguments(REGISTER "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Append sources
    get_property(CURRENT_SOURCES GLOBAL PROPERTY GLOBAL_SOURCES)
    set_property(GLOBAL PROPERTY GLOBAL_SOURCES ${CURRENT_SOURCES} ${REGISTER_SOURCES})

    # Append headers
    get_property(CURRENT_HEADERS GLOBAL PROPERTY GLOBAL_HEADERS)
    set_property(GLOBAL PROPERTY GLOBAL_HEADERS ${CURRENT_HEADERS} ${REGISTER_HEADERS})

    # Append include directories
    get_property(CURRENT_INCLUDE_DIRS GLOBAL PROPERTY GLOBAL_INCLUDE_DIRS)
    set_property(GLOBAL PROPERTY GLOBAL_INCLUDE_DIRS ${CURRENT_INCLUDE_DIRS} ${REGISTER_INCLUDE_DIRS})

    # Append definitions
    get_property(CURRENT_DEFINITIONS GLOBAL PROPERTY GLOBAL_DEFINITIONS)
    set_property(GLOBAL PROPERTY GLOBAL_DEFINITIONS ${CURRENT_DEFINITIONS} ${REGISTER_DEFINITIONS})
endfunction()

# Function to create executable with all dependencies
function(add_project_executable TARGET_NAME)
    get_property(ALL_SOURCES GLOBAL PROPERTY GLOBAL_SOURCES)
    get_property(ALL_INCLUDE_DIRS GLOBAL PROPERTY GLOBAL_INCLUDE_DIRS)
    get_property(ALL_DEFINITIONS GLOBAL PROPERTY GLOBAL_DEFINITIONS)

    add_executable(${TARGET_NAME} ${ALL_SOURCES} ${ARGN})
    target_include_directories(${TARGET_NAME} PRIVATE ${ALL_INCLUDE_DIRS})
    target_compile_definitions(${TARGET_NAME} PRIVATE ${ALL_DEFINITIONS})

    ## Debug inside the function
    # get_target_property(INCLUDES ${TARGET_NAME} INCLUDE_DIRECTORIES)
    # message("Target ${TARGET_NAME} include directories: ${INCLUDES}")
endfunction()


###########################################################################
# debug file directories
# Print all registered sources
function(print_sources)
    get_property(ALL_SOURCES GLOBAL PROPERTY GLOBAL_SOURCES)
    message("=== Registered Sources ===")
    foreach(source IN LISTS ALL_SOURCES)
        message("  ${source}")
    endforeach()
endfunction()

# Print all registered headers
function(print_headers)
    get_property(ALL_HEADERS GLOBAL PROPERTY GLOBAL_HEADERS)
    message("=== Registered Headers ===")
    foreach(header IN LISTS ALL_HEADERS)
        message("  ${header}")
    endforeach()
endfunction()

# Print all registered include directories
function(print_includes)
    get_property(ALL_INCLUDE_DIRS GLOBAL PROPERTY GLOBAL_INCLUDE_DIRS)
    message("=== Registered Include Directories ===")
    foreach(inc_dir IN LISTS ALL_INCLUDE_DIRS)
        message("  ${inc_dir}")
    endforeach()
endfunction()

# Print all registered definitions
function(print_definitions)
    get_property(ALL_DEFINITIONS GLOBAL PROPERTY GLOBAL_DEFINITIONS)
    message("=== Registered Definitions ===")
    foreach(def IN LISTS ALL_DEFINITIONS)
        message("  ${def}")
    endforeach()
endfunction()

# Print everything at once
function(print_all_registered)
    print_sources()
    print_headers() 
    print_includes()
    print_definitions()
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