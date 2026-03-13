/*
 * Created by ckalo on 3/12/26.
 */
#ifndef FILE_SYS_DEBUG_H
#define FILE_SYS_DEBUG_H

#include "sys_utils.h"


enum TYPES {
    TYPE_INT8, TYPE_CHAR, TYPE_UINT8,
    TYPE_INT16,TYPE_UINT16,
    TYPE_FLOAT, TYPE_INT32, TYPE_UINT32,
    TYPE_INT64, TYPE_UINT64, TYPE_DOUBLE, TYPE_POINTER, TYPE_FUNC_PTR,
    TYPE_CHAR_PTR,
};

typedef struct fsdebug {
    void* address;
    uint32 size;
    char data[1];
} fsdebug;



void file_sys_debug_init();

void file_sys_push(void* const value, typesize_t typesize, const enum TYPES type, void* address);

void file_sys_retrieve();

char* file_sys_retrieve_info(char* buffer);

bool file_sys_exists(void* address);




void file_sys_test();
void file_sys_test2();


#endif //FILE_SYS_DEBUG_H