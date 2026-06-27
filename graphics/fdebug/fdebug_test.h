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
    ENGINE_FDEBUG_FILE_TEST = 1,
    ENGINE_FDEBUG_FILE_VSHADER,
    ENGINE_FDEBUG_FILE_FSHADER,

    ENGINE_FDEBUG_FILE_COUNT,
};


void engine_fdebug_init();

void engine_fdebug_append_string(enum ENGINE_FDEBUG_FILE_NAMES, const char* str, const uint32 size);

char* engine_fdebug_get_string_from_arena_alloc();

void engine_fdebug_release_arena();

void engine_fdebug_write_to_file(enum ENGINE_FDEBUG_FILE_NAMES type);

char* engine_fdebug_get_filename_from_enum_type(enum ENGINE_FDEBUG_FILE_NAMES type);

#endif //GRAPHICS_TEST_FDEBUG_TEST_H