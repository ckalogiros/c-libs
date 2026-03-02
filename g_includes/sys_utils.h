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
//
// #define make_uint64_from_char8(start_char, size)\
// ((uint64)(uint8)(start_char[0]) << 56) |\
// ((uint64)(uint8)(start_char[1]) << 48) |\
// ((uint64)(uint8)(start_char[2]) << 40) |\
// ((uint64)(uint8)(start_char[3]) << 32) |\
// ((uint64)(uint8)(start_char[4]) << 24) |\
// ((uint64)(uint8)(start_char[5]) << 16) |\
// ((uint64)(uint8)(start_char[6]) <<  8) |\
// ((uint64)(uint8)(start_char[7])      )

void memset_64bit(const char* src, const uint64 val, const uint64 size);

#endif //SYS_UTILS_H
