/*
 * Created by ckalo on 3/13/26.
 */

#include "file_sys_debug.h"
#include <stdlib.h>
#include <string.h>
#include "stringc.h"
#include "file_sys_interface.h"
#include "logger.h"
#include "mem_man_interface.h"
#include "my_macros.h"
#include "string_format.h"


static void* start = NULL;
static void* debug_all_values_buffer = NULL;
static void* store_ptr = NULL;
static void* retrieve_ptr = NULL;


void
file_sys_debug_init()
{
    debug_all_values_buffer = aligned_alloc(1024, 8192);
    start = debug_all_values_buffer;
    store_ptr = debug_all_values_buffer;
    retrieve_ptr = debug_all_values_buffer;
}

void
file_sys_push(void* const value, typesize_t typesize, const enum TYPES type, void* address)
{
    if(!debug_all_values_buffer) {
        file_sys_debug_init(); // Initialize if not already done
    }
    // print("-- address:%016llx\n", address);

    fsdebug* d = (fsdebug*) debug_all_values_buffer;

    d->address = address;
    *(char*) d->data = *((char*) debug_all_values_buffer + (uint8) (sizeof(d->address) + sizeof(d->size)));

    char* ptr = &d->data[0];

    *(typesize_t*) ptr = typesize;
    ptr = (char*) ptr + sizeof(typesize_t);


    *(int*) ptr = type;
    ptr = (char*) ptr + sizeof(int);

    if(type <= TYPE_UINT8) { *(int8*) ptr = *(int8*) value; } else if(type <= TYPE_UINT16) { *(int32*) ptr = *(int32*) value; } else if(type <= TYPE_UINT32) {
        if(type == TYPE_FLOAT) { *(float*) ptr = *(float*) value; } else { *(uint32*) ptr = *(uint32*) value; }
    } else if(type <= TYPE_FUNC_PTR) {
        if(type == TYPE_DOUBLE) { *(double*) ptr = *(double*) value; } else { *(int64*) ptr = *(int64*) value; }
    } else {
        memcpy(ptr, (char*) value, typesize);
    }

    ptr = (char*) ptr + typesize;

    debug_all_values_buffer = ptr;
    store_ptr = ptr;
}

void
file_sys_retrieve()
{
    const fsdebug* d = (fsdebug*) start;
    const char* ptr = (char*) d;

    while(ptr < (char*) debug_all_values_buffer) {
        ptr = ptr + sizeof(d->address) + sizeof(d->size);

        const typesize_t typesize = *(typesize_t*) ptr;
        ptr = (char*) ptr + sizeof(typesize_t);

        const enum TYPES type = *(int*) ptr;
        ptr = (char*) ptr + sizeof(int);

        switch(type) {
            case TYPE_CHAR: {
                const char val = *(char*) ptr;
                print("INT8: %c\n", val);
                break;
            }
            case TYPE_INT8: {
                const int8_t val = *(int8_t*) ptr;
                print("INT8: %d\n", val);
                break;
            }
            case TYPE_INT16: {
                const int16_t val = *(int16_t*) ptr;
                print("INT16: %d\n", val);
                break;
            }
            case TYPE_INT32: {
                const int32_t val = *(int32_t*) ptr;
                print("INT32: %d\n", val);
                break;
            }
            case TYPE_INT64: {
                const int64_t val = *(int64_t*) ptr;
                print("INT64: %lld\n", (long long) val);
                break;
            }
            case TYPE_UINT8: {
                const uint8_t val = *(uint8_t*) ptr;
                print("UINT8: %u\n", val);
                break;
            }
            case TYPE_UINT16: {
                const uint16_t val = *(uint16_t*) ptr;
                print("UINT16: %u\n", val);
                break;
            }
            case TYPE_UINT32: {
                const uint32_t val = *(uint32_t*) ptr;
                print("UINT32: %u\n", val);
                break;
            }
            case TYPE_UINT64: {
                const uint64_t val = *(uint64_t*) ptr;
                print("UINT64: %llu\n", (unsigned long long) val);
                break;
            }
            case TYPE_FLOAT: {
                const float val = *(float*) ptr;
                print("FLOAT: %f\n", val);
                break;
            }
            case TYPE_DOUBLE: {
                const double val = *(double*) ptr;
                print("DOUBLE: %f\n", val);
                break;
            }
            case TYPE_POINTER: {
                const void* val = (void*) ptr;
                print("POINTER: %p\n", val);
                break;
            }
            case TYPE_CHAR_PTR: {
                const char* val = (char*) ptr;
                print("STRING: %s\n", val);
                break;
            }
            case TYPE_FUNC_PTR: {
                void (*val)() = (void(*)()) ptr;
                print("FUNCTION POINTER: %p\n", val);
                break;
            }
            default:
                my_exit("ERROR: retrieving value\nNO TYPE WAS DETECTED, default of the switch\n", __FILE__, __LINE__);
                break;
        }

        ptr = (char*) ptr + typesize;
    }
}

char*
file_sys_retrieve_info(char* buffer)
{
    fsdebug* d = (fsdebug*) retrieve_ptr;
    char* ptr = (char*) d;
    ptr = ptr + sizeof(d->address) + sizeof(d->size);

    const typesize_t typesize = *(typesize_t*) ptr;
    ptr = (char*) ptr + sizeof(typesize_t);

    const enum TYPES type = *(int*) ptr;
    ptr = (char*) ptr + sizeof(int);

    switch(type) {
        case TYPE_CHAR: {
            const char val = *(char*) ptr;
            string_format(buffer, "%c ", val);
            break;
        }
        case TYPE_INT8: {
            const int8_t val = *(int8_t*) ptr;
            string_format(buffer, "%d ", val);
            break;
        }
        case TYPE_INT16: {
            const int16_t val = *(int16_t*) ptr;
            string_format(buffer, "%d ", val);
            break;
        }
        case TYPE_INT32: {
            const int32_t val = *(int32_t*) ptr;
            string_format(buffer, "%d ", val);
            break;
        }
        case TYPE_INT64: {
            const int64_t val = *(int64_t*) ptr;
            string_format(buffer, "%lld ", val);
            break;
        }
        case TYPE_UINT8: {
            const uint8_t val = *(uint8_t*) ptr;
            string_format(buffer, "%u ", val);
            break;
        }
        case TYPE_UINT16: {
            const uint16_t val = *(uint16_t*) ptr;
            string_format(buffer, "%u ", val);
            break;
        }
        case TYPE_UINT32: {
            const uint32_t val = *(uint32_t*) ptr;
            string_format(buffer, "%u ", val);
            break;
        }
        case TYPE_UINT64: {
            const uint64_t val = *(uint64_t*) ptr;
            string_format(buffer, "%llu ", val);
            break;
        }
        case TYPE_FLOAT: {
            const float val = *(float*) ptr;
            string_format(buffer, "%f ", val);
            break;
        }
        case TYPE_DOUBLE: {
            const double val = *(double*) ptr;
            string_format(buffer, "%llf ", val);
            break;
        }
        case TYPE_POINTER: {
            const void* val = (void*) ptr;
            string_format(buffer, "%016x ", val);
            break;
        }
        case TYPE_CHAR_PTR: {
            const char* val = (char*) ptr;
            string_format(buffer, "{str}:%s ", val);
            break;
        }
        case TYPE_FUNC_PTR: {
            void (*val)() = (void(*)()) ptr;
            string_format(buffer, "{fn}:%016x ", val);
            break;
        }
        default:
            my_exit("ERROR: retrieving value\nNO TYPE WAS DETECTED, default of the switch\n", __FILE__, __LINE__);
            break;
    }

    ptr = (char*) ptr + typesize;
    retrieve_ptr = ptr;

    return buffer;
}

bool
file_sys_exists(void* address)
{
    fsdebug* d = (fsdebug*) retrieve_ptr;
    if(d->address == address) {
        return TRUE;
    }
    return FALSE;
}

void
file_sys_test()
{
    string_init_table(2);
    const uint8 x8 = 8;
    const uint16 x16 = 16;
    const uint32 x32 = 32;
    const uint64 x64 = 64;
    const float f = 3.14;
    const double d = 4.14;
    const char* str = "Hello!";
    char** ptr = &str;
    void* fnptr = &file_sys_test;
    uint32 size = 0;

    file_sys_push((void*)&x8, sizeof(uint8), TYPE_UINT8, (void*)&x8);
    size += sizeof(uint8);
    file_sys_push((void*)&x16, sizeof(uint16), TYPE_UINT16, (void*)&x16);
    size += sizeof(uint16);
    file_sys_push((void*)&x32, sizeof(uint32), TYPE_UINT32, (void*)&x32);
    size += sizeof(uint32);
    file_sys_push((void*)&x64, sizeof(uint64), TYPE_UINT64, (void*)&x64);
    size += sizeof(uint64);
    file_sys_push((void*)&f, sizeof(float), TYPE_FLOAT, (void*)&f);
    size += sizeof(float);
    file_sys_push((void*)&d, sizeof(double), TYPE_DOUBLE, (void*)&d);
    size += sizeof(double);
    file_sys_push((void*)str, strlen(str) + 1, TYPE_CHAR_PTR, (void*)str);
    size += strlen(str);
    file_sys_push((void*)ptr, sizeof(char*), TYPE_POINTER, (void*)ptr);
    size += sizeof(char*);
    file_sys_push((void*)fnptr, sizeof(void*), TYPE_FUNC_PTR, (void*)fnptr);
    size += sizeof(void*);

    struct fs_t* fs = fs_create("debug_with_info");
    void* from = (void*)&x8;

    from = (void*) ROUND_DOWN((uint64)from, 64);
    size = ROUND_UP(size, 64);

    fs_append_mem_with_msg(fs, from, size, "");
    // file_sys_retrieve();

}

void
file_sys_test2()
{

    // {
    //     buffer_table_init();
    //     // allocate new buffer
    //     buffer_t* buffer = buffer_create_gentype(16, sizeof(uint32));
    //
    //     uint32* data = buffer->data;
    //     for(uint32 i = 0; i < buffer->capacity; i++) {
    //         buffer_set_for_push(buffer);
    //         data[i] = i + 1;
    //         file_sys_push(&data[i], sizeof(uint32), TYPE_UINT32, &data[i]);
    //     }
    //
    //     struct fs_t* fs = fs_create("debug_with_info");
    //     void* from = buffer->data;
    //     uint64 size = buffer->capacity*buffer->typesize;
    //     fs_append_mem_with_msg(fs, from, size, "");
    // }

    { // string: create
        string_init_table(2);
        struct fs_t* fs = fs_create("debug_with_info");
        void* from = NULL;
        uint64 size = 0;
        for(uint32 i = 0; i < 4; i++) {
            // allocate new buffer
            string_t* str = string_create("HELLO WORLD!");
            file_sys_push((void*)str->data, str->len + 1, TYPE_CHAR_PTR, (void*)str);


            if(!from) from = str;
            size += str->size;
        }
        char msg[128] = {0};
        string_format(&msg[0], " size:%u", size);
        fs_append_mem_with_msg(fs, from, size, msg);
    }

}
