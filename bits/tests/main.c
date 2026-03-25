/*
 * Created by ckalo on 3/19/26.
 */

#include "bitfields.h"
#include "bits.h"
#include "logger.h"
#include "my_types.h"
#include "test_unit_bitfields.h"
#include "test_unit_bits.h"



void
test_bits_binary_operations()
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
            for(int32 i = 16 - 1; i >= 0; i--) {
                const uint8 bit = bit_is_set(val, i, uint8);
                if(bit) print("%s%llu%s", YELLOW, bit, CLEAR);
                else print("%u", bit);
                k++;
                if(k == 4) {
                    print(" ");
                    k = 0;
                }
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
            for(int32 i = 16 - 1; i >= 0; i--) {
                //                const uint8 bit = bit_toggle(val, i, uint16);
                bit_toggle(val, i, uint16);
                if((val >> i) & 1) print("%s%u%s", YELLOW, (val >> i) & 1, CLEAR);
                else print("%u", (val >> i) & 1);
                k++;
                if(k == 4) {
                    print(" ");
                    k = 0;
                }
            }
            print("\nafter: ");
            bits_print_binary(val, sizeof(val));
        }
    }
}


void
test_calc_bitmask_from_start_end_bits()
{
    {
        print("-- %sTEST 1 \n   \'test_calc_bitmask_from_start_end_bits MACRO impl%s\'\n", GREEN, CLEAR);
        const uint32 nbits = 6;
        const uint32 start_bit = 0;
        const uint32 end_bit = start_bit + nbits;
        uint32 mask = bits_calc_bitmask_from_bits(start_bit, start_bit, end_bit);

        print("%sresult mask%s\n", ry, CLEAR);
        bits_print_binary(mask, sizeof(mask));

        const uint32 expected = 0b111111;
        print("%sexpected mask%s\n", ry, CLEAR);
        bits_print_binary(expected, sizeof(expected));

        if(mask == expected) {
            print("bitfield_set(): %sPASSED%s\n", GREEN, CLEAR);
        } else
            print("bitfield_set(): %sFAILED%s\n", RED, CLEAR);

        print("\n");
    }
    {
        print("-- TEST 1 \n   \'test_calc_bitmask_from_start_end_bits MACRO impl\'\n");
        const uint32 nbits = 6;
        const uint32 start_bit = 4;
        const uint32 end_bit = start_bit + nbits;
        uint32 mask = bits_calc_bitmask_from_bits(start_bit, start_bit, end_bit);

        print("%sresult mask%s\n", ry, CLEAR);
        bits_print_binary(mask, sizeof(mask));

        const uint32 expected = 0b1111110000;
        print("%sexpected mask%s\n", ry, CLEAR);
        bits_print_binary(expected, sizeof(expected));

        if(mask == expected) {
            print("bitfield_set(): %sPASSED%s\n", GREEN, CLEAR);
        } else
            print("bitfield_set(): %sFAILED%s\n", RED, CLEAR);

        print("\n");
    }
    {
        print("-- TEST 1 \n   \'test_calc_bitmask_from_start_end_bits MACRO impl\'\n");
        const uint32 nbits = 17;
        uint32 start_bit = 11;
        const uint32 end_bit = start_bit + nbits;
        const uint32 mask = bits_calc_bitmask_from_bits(start_bit, start_bit, end_bit);

        print("%sresult mask%s\n", ry, CLEAR);
        bits_print_binary(mask, sizeof(mask));

        const uint32 expected = 0b00001111111111111111100000000000;
        print("%sexpected mask%s\n", ry, CLEAR);
        bits_print_binary(expected, sizeof(expected));

        if(mask == expected) {
            print("bitfield_set(): %sPASSED%s\n", GREEN, CLEAR);
        } else
            print("bitfield_set(): %sFAILED%s\n", RED, CLEAR);

        print("\n");
    }
}
//
// void
// test_bitfield_set()
// {
//     {
//         print("-- TEST 1 \n   \'bitfield_set function impl\'\n");
//         const uint16 val = 0b111111110000;
//         const uint16 sval = 0b1010;
//         const uint16 mask = 0b11110000;
//         const uint16 nbits = 4;
//
//         print("val\n");
//         bits_print_binary(val, sizeof(val));
//         print("sval\n");
//         bits_print_binary(sval, sizeof(sval));
//         print("mask\n");
//         bits_print_binary(mask, sizeof(mask));
//         print("nbits:%u\n", nbits);
//
//         uint16 val_masked_inv = val & (~mask);
//         uint16 sval_shifted_masked = (sval << nbits) & mask;
//         uint16 res = val_masked_inv | sval_shifted_masked;
//
//         print("val_masked_inv\n");
//         bits_print_binary(val_masked_inv, sizeof(val_masked_inv));
//         print("sval_shifted_masked\n");
//         bits_print_binary(sval_shifted_masked, sizeof(sval_shifted_masked));
//         print("%sresult%s\n", ry, CLEAR);
//         bits_print_binary(res, sizeof(res));
//
//         uint16 expected = 0b111110100000;
//         print("%sexpected result%s\n", ry, CLEAR);
//         bits_print_binary(expected, sizeof(expected));
//
//         if(res == expected) {
//             print("bitfield_set(): %sPASSED%s\n", GREEN, CLEAR);
//         } else
//             print("bitfield_set(): %sFAILED%s\n", RED, CLEAR);
//
//         print("\n");
//     }
//     {
//         print("-- TEST 2 \n   \'bitfield_set function impl\'\n");
//         const uint16 val = 0b100011110000;
//         const uint16 sval = 0b1010;
//         const uint16 mask = 0b11110000;
//         const uint16 nbits = 4;
//
//         print("val\n");
//         bits_print_binary(val, sizeof(val));
//         print("sval\n");
//         bits_print_binary(sval, sizeof(sval));
//         print("mask\n");
//         bits_print_binary(mask, sizeof(mask));
//         print("nbits:%u\n", nbits);
//
//         uint16 val_masked_inv = val & (~mask);
//         uint16 sval_shifted_masked = (sval << nbits) & mask;
//         uint16 res = val_masked_inv | sval_shifted_masked;
//
//         print("val_masked_inv\n");
//         bits_print_binary(val_masked_inv, sizeof(val_masked_inv));
//         print("sval_shifted_masked\n");
//         bits_print_binary(sval_shifted_masked, sizeof(sval_shifted_masked));
//         print("%sresult%s\n", ry, CLEAR);
//         bits_print_binary(res, sizeof(res));
//
//         const uint16 expected = 0b0000100010100000;
//         print("%sexpected result%s\n", ry, CLEAR);
//         bits_print_binary(expected, sizeof(expected));
//
//         if(res == expected) {
//             print("bitfield_set(): %sPASSED%s\n", GREEN, CLEAR);
//         } else
//             print("bitfield_set(): %sFAILED%s\n", RED, CLEAR);
//
//         print("\n");
//     }
// }
//
// void
// test_macro_impl_bitfield_set()
// {
//     {
//         print("-- TEST 1 \n   \'bitfield_set MACRO impl\'\n");
//         const uint16 val = 0b111111110000;
//         const uint16 sval = 0b1010;
//         const uint16 start_bit = 4;
//         const uint16 end_bit = 8;
//         const uint16 nbits = 4;
//         uint16 mask = 0;
//
//         // expected_mask = 0b11110000;
//         mask = bits_calc_bitmask_from_bits(mask, start_bit, end_bit);
//
//         print("val\n");
//         bits_print_binary(val, sizeof(val));
//         print("sval\n");
//         bits_print_binary(sval, sizeof(sval));
//         print("mask\n");
//         bits_print_binary(mask, sizeof(mask));
//         print("nbits:%u\n", nbits);
//
//         uint16 res = bitfield_set(val, sval, mask, start_bit);
//
//         print("%sresult%s\n", ry, CLEAR);
//         bits_print_binary(res, sizeof(res));
//
//         uint16 expected = 0b111110100000;
//         print("%sexpected result%s\n", ry, CLEAR);
//         bits_print_binary(expected, sizeof(expected));
//
//         if(res == expected) {
//             print("bitfield_set(): %sPASSED%s\n", GREEN, CLEAR);
//         } else
//             print("bitfield_set(): %sFAILED%s\n", RED, CLEAR);
//
//         print("\n");
//     }
//     {
//         print("-- TEST 2 \n   \'bitfield_set MACRO impl\'\n");
//         uint16 val = 0b100011110000;
//         uint16 sval = 0b1010;
//         uint16 mask = 0;
//         uint16 start_bit = 4;
//         uint16 end_bit = 8;
//         uint16 nbits = 4;
//
//         // expected_mask =  0b11110000;
//         mask = bits_calc_bitmask_from_bits(mask, start_bit, end_bit);
//
//         print("val\n");
//         bits_print_binary(val, sizeof(val));
//         print("sval\n");
//         bits_print_binary(sval, sizeof(sval));
//         print("mask\n");
//         bits_print_binary(mask, sizeof(mask));
//         print("nbits:%u\n", nbits);
//
//         uint16 res = bitfield_set(val, sval, mask, start_bit);
//
//         print("%sresult%s\n", ry, CLEAR);
//         bits_print_binary(res, sizeof(res));
//
//         uint16 expected = 0b0000100010100000;
//         print("%sexpected result%s\n", ry, CLEAR);
//         bits_print_binary(expected, sizeof(expected));
//
//         if(res == expected) {
//             print("bitfield_set(): %sPASSED%s\n", GREEN, CLEAR);
//         } else
//             print("bitfield_set(): %sFAILED%s\n", RED, CLEAR);
//
//         print("\n");
//     }
// }
//
// void
// temp_print_mem_structs_sizes()
// {
//     print(
//         "mem size:%u\n"
//         "mem_chunk size:%u\n"
//         "mem_used size:%u\n",
//         sizeof(struct mem),
//         sizeof(struct mem_chunk)
//     );
// }

int main(void)
{
    unit_test_runner_bits();
    unit_test_runner_bitfields();

    test_bits_binary_operations();
    test_calc_bitmask_from_start_end_bits();

    return 0;
}