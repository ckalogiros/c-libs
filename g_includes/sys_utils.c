/*
 * Created by ckalo on 1/6/26.
 */

#include "sys_utils.h"
#include "logger_color.h"
#include <stdio.h>
#include "file_sys_interface.h"

void
my_exit(const char* msg, const char* file, const uint32 line)
{
    printf("%s####################################################\n"
           "%s%s\nEXIT AT %s\nline:%u\n", RED, msg, CLEAR, file, line);
    printf("%s####################################################%s\n", RED, CLEAR);

    const char* filename = "/crash_debug.txt";
    struct fs_t* fs = fs_create(filename);
    fs_fprint_all_allocated_mem(filename);
    fs_append(fs, "CRASH EXIT\nmsg:%s", msg);

    exit(1);
}



void
memset_64bit(const char* src, const uint64 val, const uint64 size)
{
    const uint32 val_size = sizeof(uint64);
    uint64* ptr = (uint64*)src;

    for (size_t i = 0; i < size; i+= val_size) {

        *(ptr) = val;
        ptr++;
    }
}
