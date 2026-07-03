/*
 * Created by ckalo on 2/6/26.
 */
#ifndef MEMORY_SMATH_H
#define MEMORY_SMATH_H

#include "my_types.h"


#define PI      3.141592654f
#define PI_HALF 1.570796327f
#define SQRT2   1.41421356237f
#define SQRT3   1.73205080757f
#define SQRT5   2.2360679775f
#define SQRT_10_PLUS_2SQRT5  3.80422606518f
#define SQRT_10_MINUS_2SQRT5 2.35114100917f
#define PHI     1.618033988749895f   /// ((1.f + sqrt(5.f)) / 2.f)  *Golden ratio ≈ 1.618
#define INV_PHI 0.618033988749895f   /// (1.f / PHI)

#define is_power_of_two(x) ((x) != 0 && ((x) & ((x) - 1)) == 0)


static inline int32 fast_abs32(int32 x) {
    int32 mask = x >> 31;  // All 1's if negative, all 0's if positive
    return (x + mask) ^ mask;
}

static inline int64 fast_abs64(int64 x) {
    int64 mask = x >> 63;  // All 1's if negative, all 0's if positive
    return (x + mask) ^ mask;
}

void randomize_range(uint32 arr[], int m, int n);

#endif //MEMORY_SMATH_H