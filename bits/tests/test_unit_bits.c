/*
 * Created by ckalo on 3/19/26.
 */


#include <assert.h>
#include <string.h>

#include "bits.h"
#include "logger.h"
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

// bit_set             (v, bit, type)          ((v) |= ((force_type_size(v)) << ((type)bit)))
// bit_clear           (v, bit, type)          ((v) &= ~((force_type_size(v)) << ((type)bit)))
// bit_toggle          (v, bit, type)          ( (v) ^= (type) ( (force_type_size(v) ) << ( (bit)&(force_width_mask(v)) ) ) )
// bit_is_set          (v, bit, type)          (type)( ((v) >> (bit)) & (1) )
// bit_get             (v, bit)                ((((v) >> (bit)) & (1)))
// flag_is_set64       (v, flag)               ((v) & (flag))
// bits_mask_low       (v)                     ((v) & (bits_get_half_fullmask_from_type(v)))
// bits_mask_high      (v)                     ( ( (v) >> bits_get_halfsize_from_type(v) ) & (bits_get_half_fullmask_from_type(v)) )

void
test_bit_set(const uint32 num_bits)
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
test_bit_clear(const uint32 num_bits, const uint32 maxbitmask)
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
test_bit_toggle(const uint32 num_bits)
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
test_bit_is_set(const uint32 num_bits)
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
test_bit_get(const uint32 num_bits)
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
void
test_bits()
{
    /* set bit */
    {
        test_bit_set(BITS8);
        test_bit_set(BITS16);
        test_bit_set(BITS32);
        test_bit_set(BITS64);
    }

    /* clear bit */
    {
        test_bit_clear(BITS8, MAX_UINT8);
        test_bit_clear(BITS16, MAX_UINT16);
        test_bit_clear(BITS32, MAX_UINT32);
        test_bit_clear(BITS64, MAX_UINT64);
    }

    /* toggle bit */
    {
        test_bit_toggle(BITS8);
        test_bit_toggle(BITS16);
        test_bit_toggle(BITS32);
        test_bit_toggle(BITS64);
    }

    /* is bit set */
    {
        test_bit_is_set(BITS8);
        test_bit_is_set(BITS16);
        test_bit_is_set(BITS32);
        test_bit_is_set(BITS64);
    }

    /* bit get */
    {
        test_bit_get(BITS8);
        test_bit_get(BITS16);
        test_bit_get(BITS32);
        test_bit_get(BITS64);
    }

    print_test_result("test_bits()", TRUE, __FILE__, __LINE__, "");
}


/*Test Runner*/
void
unit_test_runner_bits(void)
{
    // memman_init();
    print("Running Bits Tests\n");
    print("==============================\n\n");
    test_bits();

    print("\n[%s✓✓✓%s] All tests completed\n", GREEN, CLEAR);
    // memman_terminate();
}

#undef ENABLE_PRINT_DEBUG
#undef FOR
#undef FOR_REV
