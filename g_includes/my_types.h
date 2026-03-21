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

typedef uint8 typesize_t;


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

typedef uint16 bufidx_t; // todo remove from here



/***************************************************************************************************
 * global generics
 **************************************************************************************************/
/* get the full mask (hex value) from value's type */
#define types_get_fullmask_from_type(T) \
_Generic((T),                           \
uint8   : 0xff,                         \
uint16  : 0xffff,                       \
uint32  : 0xffffffff,                   \
uint64  : 0xffffffffffffffff,           \
default : 0x0)

/* get the half of the full mask (hex value) from value's type */
#define types_get_half_fullmask_from_type(T) \
_Generic((T),                                \
uint8   : 0xf,                               \
uint16  : 0xff,                              \
uint32  : 0xffff,                            \
uint64  : 0xffffffff,                        \
default : 0x0)

/* get the number of bits from value's type */
#define types_get_bitsize_from_type(T) \
_Generic((T),                          \
uint8   : 8,                           \
uint16  : 16,                          \
uint32  : 32,                          \
uint64  : 64,                          \
default : 0)

/* get half the number of bits from value's type */
#define types_get_half_bitsize_from_type(T) \
_Generic((T),                               \
uint8   : 4,                                \
uint16  : 8,                                \
uint32  : 16,                               \
uint64  : 32,                               \
default : 0)


/* force the type size to value 1 */
#define types_force_typesize(T)  \
_Generic((T),                    \
uint8   : UINT8_C(1),            \
uint16  : UINT16_C(1),           \
uint32  : UINT32_C(1),           \
uint64  : UINT64_C(1),           \
default : UINT64_C(1))

#define types_get_bytesize_from_type(T) \
_Generic((T),                           \
uint8: 1,                               \
uint16: 2,                              \
uint32: 4,                              \
uint64: 8,                              \
default: 0)

#define types_get_type_from_bytesize(size) \
_Generic(((char(*)[size])NULL),            \
char(*)[1]:   uint8,                       \
char(*)[2]:   uint16,                      \
char(*)[4]:   uint32,                      \
char(*)[8]:   uint64,                      \
default:      void* )

/* get the number of bits from type */
#define types_get_bitsize_from_val(v) (sizeof(v) * (8))

// Create a macro that expands to the type name
#define TYPES_FROM_SIZE(size)   \
_Generic(((char(*)[size])NULL), \
char(*)[1]:   uint8_t,          \
char(*)[2]:   uint16_t,         \
char(*)[4]:   uint32_t,         \
char(*)[8]:   uint64_t,         \
default:      void)



/***************************************************************************************************
 * global structs
 **************************************************************************************************/

struct flist {
    struct flist* next;
};

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
