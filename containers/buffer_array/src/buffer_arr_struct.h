/*
 * Created by ckalo on 1/9/26.
 */
#ifndef BUFFER_ARR_BUFFER_ARR_STRUCT_H
#define BUFFER_ARR_BUFFER_ARR_STRUCT_H

#include "my_types.h"

typedef uint16 bufidx_t;



enum BUFFER_ARR_ERROR_CODE {
    NO_BUFFER_ARR_TABLE_INIT                = BIT_MASK_FROM_UINT(1),
    BUFFER_ARR_INDEX_GREATER_THAN_TAIL      = BIT_MASK_FROM_UINT(2),
    BUFFER_ARR_INDEX_GREATER_THAN_CAPACITY  = BIT_MASK_FROM_UINT(3),
    BUFFER_ARR_TAIL_GREATER_THAN_CAPACITY   = BIT_MASK_FROM_UINT(4),
    BUFFER_ARR_CAPACITY_LESS_THAN_1         = BIT_MASK_FROM_UINT(5),
    BUFFER_ARR_TABLE_ALREADY_INIT           = BIT_MASK_FROM_UINT(6),
    BUFFER_ARR_TABLE_INIT_FROM_CREATE_FUN   = BIT_MASK_FROM_UINT(7),
};

typedef struct buffer_array {
    void*    data;               // buffer start (allocations of type allocations_t)
    uint64*  free_list;          // pointer to the start of the free elems list
    uint16   free_list_capacity; // total free list uint64's (Ex. buf size=128 => flist=2 uin64)
    bufidx_t capacity;           // total buffer  elements count
    bufidx_t count;              // used elements count
    int16    tail;               // last element's index
    bufidx_t id;                 // self reference index
    typesize_t typesize;         // last element's index
} buffer_t;


/*
 * Can do this instead and have buffer and data in contiguous memory
 *
 * typedef struct buffer_arr_t {
 *    uint16 free_list_capacity;
 *    bufidx_t capacity;
 *    bufidx_t count;
 *    int16 tail;
 *    bufidx_t id;
 *    int16 typesize;
 *    uint8 flist[size/8]; // with min size=8
 *    void data[1];
 * } buffer_arr_t;
 */

#endif //BUFFER_ARR_BUFFER_ARR_STRUCT_H


/*
 * https://chatgpt.com/c/6960f433-6b2c-8331-a487-59228ac3204d
 *  For 64 bit
    uint64 free_mask;
    index = __builtin_ctzll(free_mask);
    128 slots (two uint64s)
    if (mask0 != 0)
        index = __builtin_ctzll(mask0);
    else if (mask1 != 0)
        index = 64 + __builtin_ctzll(mask1);

    GCC / Clang
    __builtin_ctz(x)
    __builtin_ctzll(x)
    MSVC
    _BitScanForward(&index, x)
    _BitScanForward64(&index, x)

    Safety rule
    ctz(0) is undefined → always check first.

    8. One subtle benefit you get for free
    Using ctz makes it trivial to:
    find next free slot
    scan multiple slots
    implement iterators over active bits
    Example:
    uint64 mask = free_mask;
    while (mask) {
        int i = ctz(mask);
        mask &= mask - 1; // clear lowest set bit
 */
