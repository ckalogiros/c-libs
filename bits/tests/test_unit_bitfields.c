/*
 * Created by ckalo on 3/19/26.
 */

#include "test_unit_bitfields.h"

/*
 * Created by ckalo on 3/19/26.
 */


#include <assert.h>
#include <string.h>

#include "bitfields.h"
#include "bits.h"
#include "logger.h"
#include "mem_man_internals.h"
#include "my_macros.h"
#include "string_format.h"

// #define ENABLE_PRINT_DEBUG
#define FOR(n) for(int32 i = 0; i < n; i++)
#define FOR_REV(n) for(int32 i = n-1; i >= 0; --i)


/***************************************************************************************************
 * HELPERS
 **************************************************************************************************/


/***************************************************************************************************
 * TESTS
 **************************************************************************************************/
// bitfield_set                 (val, sval, mask, shift) ( (val & (~mask)) | ((sval << shift) & mask) )
// bitfield_clear               (val, mask, nbits)       ( (val) &= ~(mask << nbits) )
// bitfield_get                 (val, mask, shift)       ( ((val) & (mask)) >> (shift) )
// bits_calc_bitmask_from_start (type, nbits)            ((force_type_size(type) << (nbits)) - 1 )
// bits_calc_bitmask_from_bits  (type, start, end)       (((force_type_width(type) << ((end) - (start))) - force_type_size(type)) << (start))

void
test_bitfield_set(const uint32 num_bits)
{
    uint64 bitmasks[num_bits];
    FOR(num_bits) bitmasks[i] = BIT_MASK_FROM_UINT(i);


    FOR(num_bits) {
        uint64 val = 0x0;
        bit_set(val, i, uint64);
        assert(val & bitmasks[i]);
#ifdef ENABLE_PRINT_DEBUG
        bits_print_binary(val, sizeof(uint64));
#endif
    }

    char buf[32] = {0};
    const char* name = string_format(buf, "set() #%u bit values", num_bits);
    print_test_result(name, TRUE, __FILE__, __LINE__, "");
}

void
test_bitfield_clear(const uint32 num_bits, const uint32 maxbitmask)
{
    uint64 bitmasks[num_bits]; /* reverse mask's bits */
    FOR(num_bits) bitmasks[i] = ~(BIT_MASK_FROM_UINT(i));

    FOR(num_bits) {
        uint64 val = maxbitmask;
        bit_clear(val, i, uint64);
        assert(val & bitmasks[i]);
#ifdef ENABLE_PRINT_DEBUG
        bits_print_binary(val, sizeof(uint64));
#endif
    }

    char buf[32] = {0};
    const char* name = string_format(buf, "clear() #%u bit values", num_bits);
    print_test_result(name, TRUE, __FILE__, __LINE__, "");
}

void
test_bitfield_toggle(const uint32 num_bits)
{
    uint64 bitmasks[num_bits];
    FOR(num_bits) bitmasks[i] = BIT_MASK_FROM_UINT(i);

    FOR(num_bits) {
        uint64 val = 0x0;
        bit_toggle(val, i, uint64);
        assert(val & bitmasks[i]);
#ifdef ENABLE_PRINT_DEBUG
        bits_print_binary(val, sizeof(uint64));
        bits_print_binary(bitmasks[i], sizeof(uint64));
#endif
    }

    char buf[32] = {0};
    const char* name = string_format(buf, "toggle() #%u bit values", num_bits);
    print_test_result(name, TRUE, __FILE__, __LINE__, "");
}

void
test_bitfield_is_set(const uint32 num_bits)
{
    uint64 bitmasks[num_bits];
    FOR(num_bits) bitmasks[i] = BIT_MASK_FROM_UINT(i);

    FOR(num_bits) {
        const uint32 bit = bit_is_set(bitmasks[i], i, uint64);
        assert(bit == 1);
#ifdef ENABLE_PRINT_DEBUG
        bits_print_binary(bitmasks[i], sizeof(uint8));
#endif
    }

    char buf[32] = {0};
    const char* name = string_format(buf, "is_set() #%u bit values", num_bits);
    print_test_result(name, TRUE, __FILE__, __LINE__, "");
}

void
test_bitfield_get(const uint32 num_bits)
{
    uint64 bitmasks[num_bits];
    FOR(num_bits) bitmasks[i] = BIT_MASK_FROM_UINT(i);

    FOR(num_bits) {
        const uint32 bit = bit_get(bitmasks[i], i);
        assert(bit == 1);
#ifdef ENABLE_PRINT_DEBUG
        bits_print_binary(bitmasks[i], sizeof(uint8));
#endif
    }

    char buf[32] = {0};
    const char* name = string_format(buf, "get() #%u bit values", num_bits);
    print_test_result(name, TRUE, __FILE__, __LINE__, "");
}

/* Basic Creation and Initial State Tests */
#ifndef ENABLE_DEBUG
#define hepler_bitfield_set(res, val, pos, nbits) { \
res = bitfield_set(res, val, pos, nbits);\
}
#else
#define hepler_bitfield_set(res, val, pos, nbits) { \
res = bitfield_set((res), (val), (pos), (nbits));\
print(" pos:%2u | nbits:%2u            value: ",  pos, nbits);\
bits_print_binary(val,   sizeof(val));\
print(" res:  ");\
bits_print_binary(res,   sizeof(res));\
}
#endif

void helper_bitfield_set(uint32* res, uint8 val, uint8 pos, uint8 nbits, uint32* mask, uint32* read)
{
    assert(sizeof(res)*BYTE_SIZE >= pos+nbits);
    hepler_bitfield_set(*res, val, pos, nbits)
    *mask = bits_calc_bitmask_partial(val, pos, nbits);
    *read = bitfield_read(*res, pos, nbits);
    assert((((*res)&(*mask))>>pos) == *read);
    print(" mask: "); bits_print_binary(*mask, sizeof(*mask));
    print(" read: "); bits_print_binary(*read, sizeof(*read));
}

void
test_bitfields()
{
    /* check read() bitfield reads correctly */
    {
        FOR(128){
            uint16 bitfield = 0xf000+i;
            uint16 res = bitfield_read(bitfield, 0, sizeof(bitfield)*8);
            assert(res == bitfield);
            // bits_print_binary(bitfield, sizeof(bitfield));
            // bits_print_binary(res, sizeof(res));
        }
        char buf[32] = {0};
        const char* name = string_format(buf, "bitfield_read()");
        print_test_result(name, TRUE, __FILE__, __LINE__, "");

    }
    print("\n");

    /* make sure changes in result happen within the size of source value + shift position.
     * We must fail if value + the shift does not fit in the result */

    uint32 res   = 0x0;
    uint8  val   = 0;
    uint8  pos   = 0;
    uint8  nbits = 0;
    uint32 mask  = 0x0;
    uint32 read  = 0x0;


    /* set bits */
    {
        { /* same value, same nbits, equal bitpos */
            print("%s-- same value, set equal nbits, equal position width%s\n", GREEN, CLEAR);
            res = 0x00000000;

            val = 0x9; pos = 4; nbits = 4;
            helper_bitfield_set(&res, val, pos, nbits, &mask, &read);
            val = 0x9; pos = 8; nbits = 4;
            helper_bitfield_set(&res, val, pos, nbits, &mask, &read);
            pos = 12;
            helper_bitfield_set(&res, val, pos, nbits, &mask, &read);
            pos = 16;
            helper_bitfield_set(&res, val, pos, nbits, &mask, &read);
            pos = 20;
            helper_bitfield_set(&res, val, pos, nbits, &mask, &read);
            pos = 24;
            helper_bitfield_set(&res, val, pos, nbits, &mask, &read);

            /* read() back  */
            pos = 8, nbits = 8;
            mask = bits_calc_bitmask_partial(val, pos, nbits);
            const uint32 r = bitfield_read(res, pos, nbits);
            assert(((res&mask)>>pos) == r);

            char buf[32] = {0};
            const char* name = string_format(buf, "bitfield_set()");
            print_test_result(name, TRUE, __FILE__, __LINE__, "");
        }
        print("\n");
        { /* same value, dif nbits, dif bitpos */
            print("%s-- same value, dif nbits, equal position width %s\n", GREEN, CLEAR);
            res = 0x00000000;

            val = 0xf; pos = 2; nbits = 2;
            helper_bitfield_set(&res, val, pos, nbits, &mask, &read);
            res = 0x00000000;
            val = 0xf; pos = 9; nbits = 3;
            res = 0x00000000;
            helper_bitfield_set(&res, val, pos, nbits, &mask, &read);
            val = 0xf; pos = 14; nbits = 6;
            res = 0x00000000;
            helper_bitfield_set(&res, val, pos, nbits, &mask, &read);
            val = 0xf; pos = 22; nbits = 2;
            res = 0x00000000;
            helper_bitfield_set(&res, val, pos, nbits, &mask, &read);

            /* read() back  */
            pos = 8, nbits = 8;
            const uint32 mask = bits_calc_bitmask_partial(val, pos, nbits);
            const uint32 r = bitfield_read(res, pos, nbits);
            assert(((res&mask)>>pos) == r);

            char buf[32] = {0};
            const char* name = string_format(buf, "bitfield_set()");
            print_test_result(name, TRUE, __FILE__, __LINE__, "");
        }
    }
    print("\n");


    /* clear bits */
    {
        res = 0xffffffff;
        pos = 2; nbits = 6;
        print(" clear nbits:%u | pos:%u\n", nbits, pos);
        print(" before clear - res: "); bits_print_binary(res, sizeof(res));
        bitfield_clear(res, pos, nbits);
        print(" after  clear - res: "); bits_print_binary(res, sizeof(res));

        res = 0xffffffff;
        pos = 8; nbits = 3;
        print(" clear nbits:%u | pos:%u\n", nbits, pos);
        print(" before clear - res: "); bits_print_binary(res, sizeof(res));
        bitfield_clear(res, pos, nbits);
        print(" after  clear - res: "); bits_print_binary(res, sizeof(res));

        res = 0xffffffff;
        pos = 3; nbits = 22;
        print(" clear nbits:%u | pos:%u\n", nbits, pos);
        print(" before clear - res: "); bits_print_binary(res, sizeof(res));
        bitfield_clear(res, pos, nbits);
        print(" after  clear - res: "); bits_print_binary(res, sizeof(res));

        char buf[32] = {0};
        const char* name = string_format(buf, "bitfield_clear()");
        print_test_result(name, TRUE, __FILE__, __LINE__, "");
    }
    print("\n");


    /* toggle bits */
    {
        res = 0xf0f0f0f0;
        pos = 2; nbits = 6;
        mask = bits_calc_bitmask_partial(val, pos, nbits);
        print(" clear nbits:%u | pos:%u\n", nbits, pos);
        print(" mask:               "); bits_print_binary(mask, sizeof(mask));
        print(" before clear - res: "); bits_print_binary(res, sizeof(res));
        res = bitfield_toggle(res, pos, nbits);
        print(" after  clear - res: "); bits_print_binary(res, sizeof(res));

        res = ((~res) & mask) |  (res & ~mask);
        res = 0x333335553;
        pos = 4; nbits = 8;
        mask = bits_calc_bitmask_partial(val, pos, nbits);
        print(" clear nbits:%u | pos:%u\n", nbits, pos);
        print(" mask:               "); bits_print_binary(mask, sizeof(mask));
        print(" before clear - res: "); bits_print_binary(res, sizeof(res));
        res = bitfield_toggle(res, pos, nbits);
        print(" after  clear - res: "); bits_print_binary(res, sizeof(res));

        res = ((~res) & mask) |  (res & ~mask);
        res = 0xffffffff;
        pos = 9; nbits = 28;
        mask = bits_calc_bitmask_partial(val, pos, nbits);
        print(" clear nbits:%u | pos:%u\n", nbits, pos);
        print(" mask:               "); bits_print_binary(mask, sizeof(mask));
        print(" before clear - res: "); bits_print_binary(res, sizeof(res));
        res = bitfield_toggle(res, pos, nbits);
        print(" after  clear - res: "); bits_print_binary(res, sizeof(res));


        char buf[32] = {0};
        const char* name = string_format(buf, "bitfield_toggle()");
        print_test_result(name, TRUE, __FILE__, __LINE__, "");
    }
    print("\n");

    print_test_result("test_bitfields()", TRUE, __FILE__, __LINE__, "");
}


/*Test Runner*/
void
unit_test_runner_bitfields(void)
{
    // memman_init();
    print("Running Bitfields Tests\n");
    print("==============================\n\n");


    test_bitfields();

    print("\n[%s✓✓✓%s] All tests completed\n", GREEN, CLEAR);
    // memman_terminate();
}

#undef ENABLE_PRINT_DEBUG
#undef FOR
#undef FOR_REV
