/*
 * Created by ckalo on 2/7/26.
 */
#ifndef BITS_BITS_H
#define BITS_BITS_H

#include "logger.h"
#include "my_types.h"


/* check if bit is set (returns non-zero if set) */
#define bit_is_set(v, bit, type)  ((type)(((v) >> (bit)) & (1)))

/* check if specific flag is set */
#define flag_is_set64(v, flag)    ((v) & (flag))

/* set a bit */
#define bit_set(v, bit, type)     ((v) |= (types_force_typesize(v) << bit))

/* clear a bit */
#define bit_clear(v, bit, type)   ((v) &= ~(types_force_typesize(v) << bit))

/* toggle a bit */
#define bit_toggle(v, bit, type)  ((v) ^= (type) (types_force_typesize(v)  << (bit)))

/* get single bit */
#define bit_get(v, bit)           (((v) >> (bit)) & (1))

// ISSUES: use bitfield_read() instead
/* get the upper half part of any uint */
#define bits_mask_low(v)          ((v) & (types_get_half_fullmask_from_type(v)))

/* get the lower half part of any uint */
#define bits_mask_high(v)         (((v) >> types_get_half_bitsize_from_type(v) ) & (types_get_half_fullmask_from_type(v)))


/* print-debug */
void bits_print_binary(const uint64 val, const uint32 type_size);


#endif //BITS_BITS_H
