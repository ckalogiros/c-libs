/*
 * Created by ckalo on 6/27/26.
 *
 * FILES
 * We use 1 file handler, to handle many debug files, by changing the filename.
 *
 * MEMORY
 * We use an arena to store all text.
 * The arena must be "reset", by call to "engine_fdebug_release_arena()",
 * in order to reuse it.
 * For efficient debug info, spanning through many areas in the same frame,
 * IMPLEMENT new arena_allocator instances, OR just print/write directly to a file .
 */

#include "fdebug_test.h"

#include <assert.h>
#include <stdarg.h>
#include <string.h>

#include "arena_allocator.h"
#include "mem_man_types.h"
#include "string_format.h"



struct fdebug_ {
    memid_t arena_instance_id;
    enum ENGINE_FDEBUG_FILE_NAMES file_type;
    struct fs_t* file;
    bool is_arena_in_use;
};

struct fdebug_ fdebug_table[ENGINE_FDEBUG_FILE_COUNT];
void engine_fdebug_init()
{
    uint32 start = ENGINE_FDEBUG_FILE_NULL + 1;
    for(uint32 i = start; i < ENGINE_FDEBUG_FILE_COUNT; i++) {

        char filename[64] = ENGINE_FDEBUG_DIR;
        strcat(filename, engine_fdebug_get_filename_from_enum_type(i));

        fdebug_table[i].arena_instance_id = arena_allocator_create_new_instance(8);
        fdebug_table[i].file = fs_create(filename);
        fdebug_table[i].file_type = i;
        fdebug_table[i].is_arena_in_use = FALSE;

        fs_reset(fdebug_table[i].file);

    }

}

memid_t fdebug_get_instance_id(enum ENGINE_FDEBUG_FILE_NAMES type)
{
    memid_t instance_id = fdebug_table[type].arena_instance_id;
    return instance_id;
}
struct fs_t* fdebug_get_file(enum ENGINE_FDEBUG_FILE_NAMES type)
{
    struct fs_t* fs = fdebug_table[type].file;
    return fs;
}


/// declerations
char* engine_fdebug_get_filename_from_enum_type(enum ENGINE_FDEBUG_FILE_NAMES type);



void engine_fdebug_append_string(enum ENGINE_FDEBUG_FILE_NAMES type, const char* str, const uint32 size)
{
    memid_t instance_id = fdebug_get_instance_id(type);
    char* ptr = arena_allocator_alloc(instance_id, size, sizeof(char));
    memcpy(ptr, str, size);
}
void engine_fdebug_append_string_format(enum ENGINE_FDEBUG_FILE_NAMES type, char* format, ...)
{
    memid_t instance_id = fdebug_get_instance_id(type);

    va_list va_args;
    va_start(va_args, format);

    char buffer[128] = {0}; char* ptr = buffer;
    const uint32 formated_size = string_format_va_list_call(&ptr, sizeof(buffer), format, va_args);
    char* arena_ptr = arena_allocator_alloc(instance_id, formated_size, sizeof(char));
    memcpy(arena_ptr, ptr, formated_size);

    va_end(va_args);
}

/* This is NONE destructive to the arena memory allocation */
char* engine_fdebug_get_string_from_arena_alloc(enum ENGINE_FDEBUG_FILE_NAMES type)
{
    memid_t instance_id = fdebug_get_instance_id(type);
    char* ptr = (char*)debug_arena_allocator_get_instance_data(instance_id);
    return ptr;
}

void engine_fdebug_flush_arena(enum ENGINE_FDEBUG_FILE_NAMES type)
{
    memid_t instance_id = fdebug_get_instance_id(type);
    arena_allocator_release(instance_id);
}

/***************************************************************************************************
 * WRITE DEBUG TO FILE
 **************************************************************************************************/
void engine_fdebug_append_to_file_from_arena(enum ENGINE_FDEBUG_FILE_NAMES type)
{
    struct fs_t* fs = fdebug_get_file(type);

    void* ptr = engine_fdebug_get_string_from_arena_alloc(type);
    char* str = (char*)ptr;

    char filename[64] = ENGINE_FDEBUG_DIR;
    strcat(filename, engine_fdebug_get_filename_from_enum_type(type));
    fs_change_filename(fs, filename);
    fs_append(fs, str);
}

void engine_fdebug_append_to_file_immidiate(enum ENGINE_FDEBUG_FILE_NAMES type, const char* str)
{
    struct fs_t* fs = fdebug_get_file(type);
    char filename[64] = ENGINE_FDEBUG_DIR;
    strcat(filename, engine_fdebug_get_filename_from_enum_type(type));
    fs_change_filename(fs, filename);
    fs_append(fs, str);
}

char* engine_fdebug_get_filename_from_enum_type(enum ENGINE_FDEBUG_FILE_NAMES type)
{
    switch (type) {
        case ENGINE_FDEBUG_FILE_GENERIC: return "fdebug_generic.cc";
        case ENGINE_FDEBUG_FILE_VSHADER: return "fdebug_vshader.cc";
        case ENGINE_FDEBUG_FILE_FSHADER: return "fdebug_fshader.cc";
        case ENGINE_FDEBUG_FILE_VERTEX_BUFFER:  return "fdebug_vertex_buffer.cc";
        case ENGINE_FDEBUG_FILE_OBJ_COMPONENTS: return "fdebug_obj_components.cc";
        default: return "fdebug_default.cc";
    }
}