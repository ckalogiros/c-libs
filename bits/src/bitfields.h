/*
 * Created by ckalo on 3/19/26.
 */
#ifndef BITS_BIT_FIELDS_H
#define BITS_BIT_FIELDS_H

#include "my_types.h"


/***************************************************************************************************
 * BITFIELD MASKS: partial mask (exact to nbits), full mask (from bit 0 to nbits)
 **************************************************************************************************/

/* ***********************************************
 * calculates the full mask of n bits, from bit 0.
 * Ex.                  3    F
 *      6 bits mask = 0011 1111, hex 0x3F        */
#define bits_calc_bitmask_from_start(type, nbits) ((types_force_typesize(type) << (nbits)) - 1 )


/* *********************************************
 * calculates the partial mask of n bits, from bit 'pos'.
 * Ex.                         val:2
 *      4 bits mask, at pos = 4 -->  ...      0000 1111 0000
 *      6 bits mask, at pos = 8 -->  ... 0011 1111 0000 0000 */
#define bits_calc_bitmask_partial(val, pos, nbits) ( ((types_force_typesize(val)<<nbits)-1) << pos )

/* *********************************************
 * Create mask for bits [start, end] inclusive */
#define bits_calc_bitmask_from_bits(type, start, end) \
( ( (types_force_typesize(type) << ((end) - (start))) - types_force_typesize(type)) << (start) )

#define bits_calc_bitmask_from_type(type) \
((types_force_typesize(type) << types_get_bitsize_from_type(type)) - 1 )




/***************************************************************************************************
 * BITFIELD OPERATIONS: set(), clear(), read(), toggle
 **************************************************************************************************/
/* *********************************************
 * 1111 1111 1111 1111 1111 1111 1111 1111 result
 * 0010 value
 * width: 4 bits
 * pos:   8 bits
 *
 * calculate mask
 * 1111 mask with width:4 bits
 * calculate masks position: mask << pos: 8 bits
 * 0000 0000 0000 0000 0000 1111 0000 0000 mask
 *
 * exclude result's other bits, by inverted mask
 * 1111 1111 1111 1111 1111 0000 1111 1111 mask  0000 is the width of the value to be set
 *
 * and then OR it with the shifted value masked by nbits
 * 1111 1111 1111 1111 1111 0010 1111 1111 result
 */
#define bitfield_set(res, val, pos, nbits) \
            ( (res & ~bits_calc_bitmask_partial(val, pos, nbits)) | \
            ( (val) << (pos))&bits_calc_bitmask_partial(val, pos, nbits) )

#define bitfield_read(val, pos, nbits) \
            ( (val & bits_calc_bitmask_partial(val, pos, nbits)) >> (pos) )


/* *****************************************************************
 * clear nbits = 6
 *
 * 0000 0000 0000 0000 1111 1111 1111 1111 value
 * 0000 0000 0000 0000 0000 0000 0011 1111 mask
 *
 * 0000 0000 0000 0000 1111 1111 1100 0000  mask << nbits
 * 1111 1111 1111 1111 1111 1111 11(00 0000)  invert with ~,
 * but because of << nbits, the last first 6 bits remain untouched */
// #define bitfield_clear(val, mask, nbits) ( (val) &= ~(mask << nbits) )
#define bitfield_clear(val, pos, nbits) ( (val) &= ~(bits_calc_bitmask_partial(val, pos, nbits)) )


#define bitfield_toggle(res, pos, nbits) \
((~res) & bits_calc_bitmask_partial(val, pos, nbits)) |  (res & ~bits_calc_bitmask_partial(val, pos, nbits))


#endif //BITS_BIT_FIELDS_H