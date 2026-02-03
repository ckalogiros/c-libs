/*
 * Created by ckalo on 1/7/26.
 *
 * CAUTION!
 * bit is printed to cmd from right to left
 */
#ifndef BITS_H
#define BITS_H

#include "my_types.h"


/* HALF TYPES */
//#define half_type_t(T) _Generic((T), \
//uint8 : uint8, \
//uint16 : uint8, \
//uint32 : uint16, \
//uint64 : uint32, \
//default : uint16)

//#define ttt(T) (u##T)

//#define cast(x) _Generic((x), \
//uint8 : (uint8)(x), \
//uint16 : (uint16)(x), \
//uint32 : (uint32)(x), \
//uint64 : (uint64)(x), \
//)

/* TYPES */
//#define type_t(T) _Generic((T), \
//uint8 : uint8, \
//uint16 : uint16, \
//uint32 : uint32, \
//uint64 : uint64, \
//default : uint32)

/* MASKS */
#define half_type_mask(T) _Generic((T), \
uint8 : 0xff, \
uint16 : 0xff, \
uint32 : 0xffff, \
uint64 : 0xffffffff, \
default : 0xffffffff)

/* BITS */
#define type_bits(T) _Generic((T), \
uint8 : 8, \
uint16 : 16, \
uint32 : 32, \
uint64 : 64, \
default : 32)
#define half_type_bits(T) _Generic((T), \
uint8 : 4, \
uint16 : 8, \
uint32 : 16, \
uint64 : 32, \
default : 16)

// get number of bits from type
#define type_size_bits(v) (sizeof(v) * (8))

/* force value literal */
#define force_value_width(T) _Generic((T), \
uint8 : UINT8_C(1), \
uint16 : UINT16_C(1), \
uint32 : UINT32_C(1), \
uint64 : UINT64_C(1), \
default : 16)
/* mask unecessary bits */
#define force_width_mask(T) _Generic((T), \
uint8 : 8, \
uint16 : 16, \
uint32 : 32, \
uint64 : 64, \
default : 32)


// check if bit is set (returns non-zero if set)
#define bit_is_set(v, bit, type)  (type)( ((v) >> (bit)) & (1) )

// check if specific flag is set
#define flag_is_set64(v, flag)  ((v) & (flag))

// set a bit
#define bit_set(v, bit, type)  ((v) |= ((force_value_width(v)) << ((type)bit)))

// clear a bit
#define bit_clear(v, bit, type)  ((v) &= ~((force_value_width(v)) << ((type)bit)))

// toggle a bit
#define bit_toggle(v, bit, type)  ( (v) ^= (type) ( (force_value_width(v) ) << ( (bit)&(force_width_mask(v)) ) ) )

// get single bit
#define bit_get(v, bit) ((((v) >> bit) & (1)))

// get the upper half part of any uint
#define bits_mask_low(v) ((v) & (half_type_mask(v)))

// get the lower half part of any uint
#define bits_mask_high(v) ( ( (v) >> half_type_bits(v) ) & (half_type_mask(v)) )


// print-debug
void
bits_print_binary(uint64 val, uint32 type_size);

void
test_bits_binary_operations();


#endif //BITS_H
