/*
 * Created by ckalo on 6/27/26.
 */
#ifndef GRAPHICS_TEST_FDEBUG_TEST_H
#define GRAPHICS_TEST_FDEBUG_TEST_H

#include "file_sys_interface.h"
// #include ""

#define ENGINE_FDEBUG_DIR "graphics/fdebug/fdebug_output/"

#define ENGINE_FDEBUG_DEFAULT_DIR ENGINE_FDEBUG_DIR "fdebug_default.cc"

enum ENGINE_FDEBUG_FILE_NAMES {
    ENGINE_FDEBUG_FILE_NULL = 0,
    ENGINE_FDEBUG_FILE_GENERIC = 1,
    ENGINE_FDEBUG_FILE_VSHADER,
    ENGINE_FDEBUG_FILE_FSHADER,
    ENGINE_FDEBUG_FILE_VERTEX_BUFFER,
    ENGINE_FDEBUG_FILE_OBJ_COMPONENTS,

    ENGINE_FDEBUG_FILE_COUNT,
};


void engine_fdebug_init();

void engine_fdebug_append_string(enum ENGINE_FDEBUG_FILE_NAMES type, const char* str, const uint32 size);

void engine_fdebug_append_string_format(enum ENGINE_FDEBUG_FILE_NAMES type, char* format, ...);

char* engine_fdebug_get_string_from_arena_alloc(enum ENGINE_FDEBUG_FILE_NAMES type);

void engine_fdebug_flush_arena(enum ENGINE_FDEBUG_FILE_NAMES type);

void engine_fdebug_append_to_file_from_arena(enum ENGINE_FDEBUG_FILE_NAMES type);

void engine_fdebug_append_to_file_immidiate(enum ENGINE_FDEBUG_FILE_NAMES type, const char* str);

char* engine_fdebug_get_filename_from_enum_type(enum ENGINE_FDEBUG_FILE_NAMES type);

#endif //GRAPHICS_TEST_FDEBUG_TEST_H