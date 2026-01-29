/*
 * Created by ckalo on 1/7/26.
 */

#include "bits.h"
#include "logger.h"

void bits_print_binary(const uint64 val, const uint32 type_size)
{

    print("    read: right to left <---------");
    const int32 num_bits = (int32)(type_size * 8);
    for (int32 i = num_bits-1; i >= 0; i--)
    {
        const uint8 bit = bit_get(val, i);

        if ((i+1)%16 == 0)
            print("\n    byte:[%u]    ", i/16);

        if (bit) print("%s%u%s", YELLOW, bit, CLEAR);
        else print("%u", bit);

        if (i%4 == 0)
            print(" ");

    }
    print("\n");
    print("  hex:%s%llx%s\n", br2, val, CLEAR);

}


void test_bits_binary_operations()
{
    { // clear bit
        uint16 val = 0;
        print("-- %sTEST BIT OPERATIONS%s --\n", ry, CLEAR);

        print("%sval=  hex:%x  dec:%d%s\n", yg, val, val, CLEAR);

        print("\n");

        // bit set
        {
            print("%sbit_set: 0 5 10 15%s\n", ry, CLEAR);
            bit_set(val, 0, uint16);
            bit_set(val, 5, uint16);
            bit_set(val, 10, uint16);
            bit_set(val, 15, uint16);
            bits_print_binary(val, sizeof(val));
            //        bits_print_binary(~val, sizeof(val));
        }

        print("\n");

        // bit high-low mask
        {
            print("%sbits_mask_low:%s\n", ry, CLEAR);
            const uint16 low = bits_mask_low(val);
            bits_print_binary(low, sizeof(low));
            print("%sbits_mask_high:%s\n", ry, CLEAR);
            const uint16 high = bits_mask_high(val);
            bits_print_binary(high, sizeof(high));
        }

        print("\n");
        val = 0xffff;
        print("%sval=  hex:%x  dec:%d%s\n", yg, val, val, CLEAR);
        bits_print_binary(val, sizeof(val));

        // bit clear
        {
            print("\n");

            print("%sbit_clear: 0 5 10 15%s\n", ry, CLEAR);
            bit_clear(val, 0, uint16);
            bit_clear(val, 5, uint16);
            bit_clear(val, 10, uint16);
            bit_clear(val, 12, uint16);
            bit_clear(val, 13, uint16);
            bit_clear(val, 14, uint16);
//            bit_clear(val, 15, uint16);
            bits_print_binary(val, sizeof(val));
        }

        print("\n");

        // is bit set?
        {
            print("%sis bit set:%s\n        ", ry, CLEAR);
            uint32 k = 0;
            for (int32 i = 16-1; i >= 0; i--)
            {
                const uint8 bit = bit_is_set(val, i, uint8);
                if (bit) print("%s%llu%s", YELLOW, bit, CLEAR);
                else print("%u", bit);
                k++;
                if (k == 4) { print(" "); k=0; }
            }
            print("\n");
            bits_print_binary(val, sizeof(val));
        }

        print("\n");

        // bit toggle
        {
            print("%sbit toggle:%s\nbefore: ", ry, CLEAR);
            bits_print_binary(val, sizeof(val));
            print("toggle:         ");
            uint32 k = 0;
            for (int32 i = 16-1; i >= 0; i--)
            {
//                const uint8 bit = bit_toggle(val, i, uint16);
                bit_toggle(val, i, uint16);
                if ((val >> i)&1) print("%s%u%s", YELLOW, (val >> i)&1, CLEAR);
                else print("%u", (val >> i)&1);
                k++;
                if (k == 4) { print(" "); k=0; }
            }
            print("\nafter: ");
            bits_print_binary(val, sizeof(val));
        }
    }
}

