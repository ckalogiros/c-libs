/*
 * Created by ckalo on 2/7/26.
 */
#ifndef MEMORY_MEM_STRUCTS_H
#define MEMORY_MEM_STRUCTS_H

#include "my_types.h"
#include "bits.h"


#define PAGES_PER_CHUNK 32

#define MEM_OFF_BOUNDARIES_CHECK 0x0abccbcabeffdfdac


/* *****************************************************************
 * clear nbits = 6
 *
 * 0000 0000 0000 0000 1111 1111 1111 1111 value
 * 0000 0000 0000 0000 0000 0000 0011 1111 mask
 *
 * 0000 0000 0000 0000 1111 1111 1100 0000  mask << nbits
 * 1111 1111 1111 1111 1111 1111 11(00 0000)  invert with ~,
 * but because of << nbits, the last first 6 bits remain untouched */
#define bitfield_clear(val, mask, nbits) ( (val) &= ~(mask << nbits) )

/* *****************************************************************
 * set nbits = 4
 *
 * 0110 1001 0100 1110 (original) value
 * 0000 0000 0000 0101 (store) value
 * 0000 0000 1111 0000 mask
 *
 * invert mask
 * 1111 1111 0000 1111 mask
 *
 * 'AND' inverted mask with original value, not touching other bits
 * 0110 1001 0100 1110 (original) value
 *                    AND
 * 1111 1111 0000 1111 mask
 * 0110 1001 0000 1110 (original) value masked
 *
 * LEFT-SHIFT value to proper bit position
 * 0000 0000 0000 0101 (store) value << nbits
 * 0000 0000 0101 0000 shifted (store) value
 *
 *
 * 0110 1001 0000 1110 (original) value masked
 *                    OR
 * 0000 0000 0101 0000 shifted (store) value
 * 0110 1001 0101 0101 final result
 *
 */
#define bitfield_set(val, sval, mask, shift)\
    ( (val & (~mask)) | ((sval << shift) & mask) )

#define bitfield_get(val, mask, shift) \
    ( ((val) & (mask)) >> (shift) )


/* ***********************************************
 * calculates the full mask of n bits, from bit 0.
 * Ex.                  3    F
 *      6 bits mask = 0011 1111, hex 0x3F        */
#define bits_calc_bitmask(type, nbits) ( (force_type_width(type) << (nbits)) - 1 )


/* *********************************************
 * Create mask for bits [start, end] inclusive */
#define bits_calc_bitmask_from_bits(type, start, end) \
( ( (force_type_width(type) << ((end) - (start))) - force_type_width(type)) << (start) )


/* *********************************
 * memory table index */
typedef uint16 memid_t;

/* *********************************
 * memory pages index */
typedef int16 pageid_t;


struct mem {
    void* start; // allocated start address
    struct flist* flist;
    const char* name;
    uint64 size; // total allocated size
    uint64 data_size; // total data allocated size
    uint16 bound_check_size; // bound_check allocated size
    pageid_t num_pages; // number of allocated pages
};


#endif //MEMORY_MEM_STRUCTS_H
