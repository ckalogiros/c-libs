/*
 * Created by ckalo on 1/6/26.
 */

#include "sparse_arr_log.h"
#include "bits.h"
#include "sparse_arr_internals.h"
#include "logger.h"
#include "sys_utils.h"

#define DEBUG_PRINT_BUFFER
//#undef DEBUG_PRINT_BUFFER // undef to remove print info

void
sparse_arr_print_uint32(sparse_arr_print_params params)
{
#ifdef DEBUG_PRINT_BUFFER

    sparse_arr_t* const buffer = params.buffer;
    const uint32 print_num_elems = params.print_num_elems;

    print("-- %sbuffer%s --\n", ry, CLEAR);
    print(
        "    count:%s%u%s "
        "  tail:%s%d%s "
        "  capacity:%s%u%s\n"
        "    data:%s%p%s "
        "  addr:%s%p%s\n",
        CYAN, buffer->count, CLEAR,
        CYAN, buffer->tail, CLEAR,
        CYAN, buffer->capacity, CLEAR,
        GREEN, REDUCE_ADDR(&(buffer->data), F6), CLEAR,
        GREEN, REDUCE_ADDR(&buffer, F6), CLEAR
    );

    uint64* free_list = buffer->free_list;
    print(" -PRINT free list bitmap\n");
    for(uint32 i = 0; i < buffer->free_list_capacity; i++) {
        bits_print_binary(free_list[i], sizeof(uint64));
    }

    uint32* data = buffer->data;
    print("    ");
    for(uint32 i = 0; i < print_num_elems; i++) {
        print(
            "[%s%u%s]=%s%u%s, ",
            YELLOW, i, CLEAR,
            CYAN, data[i], CLEAR
        );

        if((i + 1) % 3 == 0) print("\n    ");
    }
    print("\n");

    print("-- %sbuffer%s --\n\n", ry, CLEAR);
#endif
}

void
sparse_arr_print_free_list(const sparse_arr_t* const buffer)
{
    uint64* free_list = buffer->free_list;
    print(" -PRINT free list bitmap\n");
    for(uint32 i = 0; i < buffer->free_list_capacity; i++) {
        bits_print_binary(free_list[i], sizeof(uint64));
    }
}

void
sparse_arr_print_char(const sparse_arr_t* const buffer) {}


// ERROR handling
void
sparse_arr_print_error(const enum SPARSE_ARR_ERROR_CODE code)
{
    switch(code) {
        case NO_SPARSE_ARR_TABLE_INIT: {
            print("%sERROR:\n"
                  "Buffer table is uninitialized\n"
                  "Call sparse_arr_init_table()%s\n",
                  RED, CLEAR
            );
            my_exit("", __FILE__, __LINE__);
        }
        case SPARSE_ARR_INDEX_GREATER_THAN_TAIL: {
            print("%sERROR:\n"
                  "trying to make use of index\n"
                  "that is greater than the buffer tail%s\n",
                  RED, CLEAR
            );
            break;
        }
        case SPARSE_ARR_INDEX_GREATER_THAN_CAPACITY: {
            print("%sERROR:\n"
                  "trying to make use of index\n"
                  "that is greater than the buffer capacity%s\n",
                  RED, CLEAR
            );
            break;
        }
        case SPARSE_ARR_TAIL_GREATER_THAN_CAPACITY: {
            print("%sERROR:\n"
                  "buffer tail is greater than the buffer capacity%s\n",
                  RED, CLEAR
            );
            break;
        }
        case SPARSE_ARR_CAPACITY_LESS_THAN_1: {
            print("%sERROR:\n"
                  "buffer capacity is 0 or negative number%s\n",
                  RED, CLEAR
            );
            break;
        }
        case SPARSE_ARR_TABLE_ALREADY_INIT: {
            print("%sERROR:\n"
                  "buffer table already initialized%s\n",
                  RED, CLEAR
            );
            break;
        }
        case SPARSE_ARR_TABLE_INIT_FROM_CREATE_FUN: {
            print("%sWARNING:\n"
                  "buffer table initialized from sparse_arr_create()%s\n",
                  YELLOW, CLEAR
            );
            break;
        }
    }

    // my_exit("sparse_arr_print_error", __FILE__, __LINE__);
}
