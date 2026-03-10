/*
 * Created by ckalo on 1/6/26.
 */
#ifndef SYS_UTILS_H
#define SYS_UTILS_H

#include "my_types.h"


/*
 * copy-cat from stdlib.h
 */

/* Abort execution and generate a core-dump.  */
extern void abort(void) __THROW __attribute__ ((__noreturn__));

/* Register a function to be called when `exit' is called.  */
extern int atexit(void (*__func)(void)) __THROW __nonnull((1));

#if defined __USE_ISOC11 || defined __USE_ISOCXX11
/* Register a function to be called when `quick_exit' is called.  */
# ifdef __cplusplus
extern "C++" int
at_quick_exit(void (*__func)(void))
__THROW __asm ("at_quick_exit") __nonnull((1));
# else
extern int at_quick_exit(void (*__func)(void)) __THROW __nonnull((1));
# endif
#endif

#ifdef	__USE_MISC
/* Register a function to be called with the status
   given to `exit' and the given argument.  */
extern int on_exit(void (*__func)(int __status, void* __arg), void* __arg)
__THROW __nonnull((1));
#endif
/* We define these the same for all machines.
   Changes from this to the outside world should be done in `_exit'.  */
#define	EXIT_FAILURE	1	/* Failing exit status.  */
#define	EXIT_SUCCESS	0	/* Successful exit status.  */
/* Call all functions registered with `atexit' and `on_exit',
   in the reverse of the order in which they were registered,
   perform stdio cleanup, and terminate program execution with STATUS.  */
extern void exit(int __status) __THROW __attribute__ ((__noreturn__));

/*
 * end of copy-cat from stdlib.h
 */

void my_exit(const char* msg, const char* file, const uint32 line);



static inline uint64 make_uint64_from_char8(const char start_char[8]) {
    uint64 result = 0;
    for (int i = 0; i < 8; i++) {
        result = (result << 8) | (uint8)start_char[i];
    }
    return result;
}


/*
* Masks
*/
#define MASK_UINT64 0xFFFFFFFFFFFFFFFF

#define MASK_BYTES1 0xFF
#define MASK_BYTES2 0xFFFF
#define MASK_BYTES3 0xFFFFFF
#define MASK_BYTES4 0xFFFFFFFF
#define MASK_BYTES5 0xFFFFFFFFFF
#define MASK_BYTES6 0xFFFFFFFFFFFF
#define MASK_BYTES7 0xFFFFFFFFFFFFFF
#define MASK_BYTES8 0xFFFFFFFFFFFFFFFF

#define MASK_BYTES1_HALF 0xF
#define MASK_BYTES2_HALF 0xFFF
#define MASK_BYTES3_HALF 0xFFFFF
#define MASK_BYTES4_HALF 0xFFFFFFF
#define MASK_BYTES5_HALF 0xFFFFFFFFF
#define MASK_BYTES6_HALF 0xFFFFFFFFFFF
#define MASK_BYTES7_HALF 0xFFFFFFFFFFFFF
#define MASK_BYTES8_HALF 0xFFFFFFFFFFFFFFF



#define MASK_WORD1 0x00000000000000FF
#define MASK_WORD2 0x000000000000FF00
#define MASK_WORD3 0x0000000000FF0000
#define MASK_WORD4 0x00000000FF000000
#define MASK_WORD5 0x000000FF00000000
#define MASK_WORD6 0x0000FF0000000000
#define MASK_WORD7 0x00FF000000000000
#define MASK_WORD8 0xFF00000000000000

#define MASK_BYTE1  0x000000000000000F
#define MASK_BYTE2  0x00000000000000F0
#define MASK_BYTE3  0x0000000000000F00
#define MASK_BYTE4  0x000000000000F000
#define MASK_BYTE5  0x00000000000F0000
#define MASK_BYTE6  0x0000000000F00000
#define MASK_BYTE7  0x000000000F000000
#define MASK_BYTE8  0x00000000F0000000
#define MASK_BYTE9  0x0000000F00000000
#define MASK_BYTE10 0x000000F000000000
#define MASK_BYTE11 0x00000F0000000000
#define MASK_BYTE12 0x0000F00000000000
#define MASK_BYTE13 0x000F000000000000
#define MASK_BYTE14 0x00F0000000000000
#define MASK_BYTE15 0x0F00000000000000
#define MASK_BYTE16 0xF000000000000000

#define MASK_BYTE1_HALF  0x0000000000000007
#define MASK_BYTE2_HALF  0x0000000000000070
#define MASK_BYTE3_HALF  0x0000000000000700
#define MASK_BYTE4_HALF  0x0000000000007000
#define MASK_BYTE5_HALF  0x0000000000070000
#define MASK_BYTE6_HALF  0x0000000000700000
#define MASK_BYTE7_HALF  0x0000000007000000
#define MASK_BYTE8_HALF  0x0000000070000000
#define MASK_BYTE9_HALF  0x0000000700000000
#define MASK_BYTE10_HALF 0x0000007000000000
#define MASK_BYTE11_HALF 0x0000070000000000
#define MASK_BYTE12_HALF 0x0000700000000000
#define MASK_BYTE13_HALF 0x0007000000000000
#define MASK_BYTE14_HALF 0x0070000000000000
#define MASK_BYTE15_HALF 0x0700000000000000
#define MASK_BYTE16_HALF 0x7000000000000000


#define HEX 16
#define HEX_BYTE1 HEX
#define HEX_BYTE2 (HEX * HEX_BYTE1)
#define HEX_BYTE3 (HEX * HEX_BYTE2)
#define HEX_BYTE4 (HEX * HEX_BYTE3)
#define HEX_BYTE5 (HEX * HEX_BYTE4)
#define HEX_BYTE6 (HEX * HEX_BYTE5)
#define HEX_BYTE7 (HEX * HEX_BYTE6)
#define HEX_BYTE8 (HEX * HEX_BYTE7)


#define DIGITS1 0x1
#define DIGITS2 0xA
#define DIGITS3 0x64
#define DIGITS4 0x3E8
#define DIGITS5 0x2710
#define DIGITS6 0x186A0
#define DIGITS7 0xF4240
#define DIGITS8 0x989680

void memset_64bit(const char* src, const uint64 val, const uint64 size);
uint32 ReverseBits32(uint32 value);
uint64 ReverseBits64(uint64 value);

#endif //SYS_UTILS_H
