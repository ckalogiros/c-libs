#include <stdio.h>

#include "my_types.h"

// Set of macros for bit manipulation
#define BITMASK(N) ((1U << (N)) - 1)  // N-bit mask

#define GET_BITS(VAL, SHIFT, WIDTH) \
(((VAL) >> (SHIFT)) & BITMASK(WIDTH))

#define SET_BITS(VAR, SHIFT, WIDTH, VAL) \
(VAR) = ((VAR) & ~(BITMASK(WIDTH) << (SHIFT))) | \
(((VAL) & BITMASK(WIDTH)) << (SHIFT))

typedef enum {false, true} boole;


int
main(void)
{


    // Usage
    uint32 packed = 0;
    SET_BITS(packed, 0, 4, 2);     // Set version=2
    SET_BITS(packed, 4, 4, 5);     // Set type=5
    uint8 version = GET_BITS(packed, 0, 4);  // Get version

    bool a;
    a = false;

    printf("version: %d\n", version);

    uint32 xid = 17;
    uint32 x_mask_pos = 0x3f;
    uint32 yid = 5;

    uint32 bit = 0;

    bit |=


    return 0;
}