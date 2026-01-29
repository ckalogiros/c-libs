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


#define HEX_8 0xff
#define HEX_16 0xffff
#define HEX_32 0xffffffff
#define HEX_64 0xffffffffffffffff


#define BIT_MASK_FROM_UINT(n) (1ULL << (n))

#endif // MY_TYPES_H

#ifndef MEMORY_MANAGER_H_TEMP
#define MEMORY_MANAGER_H_TEMP

#define PAGE_SIZE 1024

// MAX page id address: ~67 megabyte in a 1024 page size.
// That is <256 int32 elements per page
// That is <128 int64 elements per page
// That is <512 int16 elements per page
typedef uint16 pageid_t;

typedef uint16 bufidx_t;


#define get_type_bytes(T) _Generic((T), uint8: 1, uint16: 2, uint32: 4, uint64: 8 )

#endif // MEMORY_MANAGER_H_TEMP
