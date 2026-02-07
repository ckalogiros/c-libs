#ifndef MY_TYPES_H
#define MY_TYPES_H

#include <stdint.h>

#define TRUE 1
#define FALSE 0
#define INT_NULL (-1)
#ifndef NULL
#define NULL ((void *) 0)
#endif

typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned int uint32;
typedef unsigned long long int uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8 bool;


#define F4 0xffff
#define F5 0xfffff
#define F6 0xffffff
// reduce the hex digits of an address
#define REDUCE_ADDR(addr, hex_mask) ((uint64) (addr) & (hex_mask))


#define BITS8 8
#define BITS16 16
#define BITS32 32
#define BITS64 64

#define BYTE_SIZE 8


#define MAX_UINT8 0xff
#define MAX_UINT16 0xffff
#define MAX_UINT32 0xffffffff
#define MAX_UINT64 0xffffffffffffffff

#define BIT_MASK_FROM_UINT(n) (1ULL << (n))

#define GREATER_THAN(a, b) (((a) > (b)) ? (a) : (b))
#define LESS_THAN(a, b)    (((a) < (b)) ? (a) : (b))

#define PAGE_SIZE 1024

typedef uint16 pageid_t;

typedef uint16 bufidx_t;

#define get_type_bytes(T) _Generic((T), uint8: 1, uint16: 2, uint32: 4, uint64: 8 )

#endif // MY_TYPES_H


// // For IDs/counters (usually positive)
// #define INVALID_ID 0
// #define NO_INDEX -1
// #define UNUSED_SLOT 0xFFFFFFFF
//
// // For array indices
// #define INDEX_NONE -1
// ssize_t index = INDEX_NONE;
//
// // For handles
// #define INVALID_HANDLE 0
// #define NULL_HANDLE ((void*)0)
// 2. Use functions for validation:
// c
//
// static inline bool is_valid_id(uint32_t id) {
//     return id != USER_ID_NONE;
// }
//
// static inline bool is_index_valid(ssize_t idx) {
//     return idx >= 0;
// }
//
// static inline bool is_handle_valid(void* handle) {
//     return handle != NULL;
// }