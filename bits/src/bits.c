/*
 * Created by ckalo on 2/7/26.
 */

#include "bits.h"


void
bits_print_binary(const uint64 val, const uint32 type_size)
{
    // print("hex:%s%3x%s: ", br2, val, CLEAR);
    const int32 num_bits = (int32) (type_size * 8);
    for(int32 i = num_bits - 1; i >= 0; i--) {
        const uint8 bit = bit_get(val, i);

        if(bit) print("%s%u%s", YELLOW, bit, CLEAR);
        else print("%u", bit);

        if(i % 4 == 0) print(" ");
        // if(i % 32 == 0) print("\n         ");
        if(i % 32 == 0 && i < num_bits-1) print("\n");
    }
    // print("\n");
}

