/*
 * Created by ckalo on 6/27/26.
 */

#include "fdebug_test.h"

#include <string.h>

#include "arena_allocator.h"
#include "mem_man_types.h"


char* engine_fdebug_get_filename_from_enum_type(enum ENGINE_FDEBUG_FILE_NAMES type);


struct fs_t* fs = NULL;
memid_t instance_id = INT_NULL;


void engine_fdebug_init()
{
    instance_id = arena_allocator_create_new_instance(8);

    fs = fs_create(ENGINE_FDEBUG_DEFAULT_DIR);

    for(uint32 i = 0; i < ENGINE_FDEBUG_FILE_COUNT; i++) {
        // const char* filename = engine_fdebug_get_filename_from_enum_type(i);


        // void* ptr = engine_fdebug_get_string_from_arena_alloc();
        // char* str = (char*)ptr;

        char filename[64] = ENGINE_FDEBUG_DIR;
        // const char* filename = engine_fdebug_get_filename_from_enum_type(type);
        strcat(filename, engine_fdebug_get_filename_from_enum_type(i));
        fs_change_filename(fs, filename);
        fs_reset(fs);
    }
}

void engine_fdebug_append_string(enum ENGINE_FDEBUG_FILE_NAMES, const char* str, const uint32 size)
{
    char* ptr = arena_allocator_alloc(instance_id, size, sizeof(char));
    memcpy(ptr, str, size);
}

/* This is NONE destructive to the arena memory allocation */
char* engine_fdebug_get_string_from_arena_alloc()
{
    char* ptr = (char*)debug_get_arena_allocator_instance_start_address(instance_id);
    return ptr;
}

void engine_fdebug_release_arena()
{
    arena_allocator_release(instance_id);
}

/***************************************************************************************************
 * WRITE DEBUG TO FILE
 **************************************************************************************************/
void engine_fdebug_write_to_file(enum ENGINE_FDEBUG_FILE_NAMES type)
{
    // void* ptr = arena_allocator_release(instance_id);
    void* ptr = engine_fdebug_get_string_from_arena_alloc();
    char* str = (char*)ptr;

    char filename[64] = ENGINE_FDEBUG_DIR;
    // const char* filename = engine_fdebug_get_filename_from_enum_type(type);
    strcat(filename, engine_fdebug_get_filename_from_enum_type(type));
    fs_change_filename(fs, filename);
    fs_append(fs, str);
}

char* engine_fdebug_get_filename_from_enum_type(enum ENGINE_FDEBUG_FILE_NAMES type)
{
    switch (type) {
        case ENGINE_FDEBUG_FILE_TEST:    return "fdebug_test.cc";
        case ENGINE_FDEBUG_FILE_VSHADER: return "fdebug_vshader.cc";
        case ENGINE_FDEBUG_FILE_FSHADER: return "fdebug_fshader.cc";
        default: return "fdebug_default.cc";
    }
}