/*
 * Created by ckalo on 2/6/26.
 */
#ifndef MEMORY_SMATH_H
#define MEMORY_SMATH_H

#include "my_types.h"

static inline int32 fast_abs32(int32 x) {
    int32 mask = x >> 31;  // All 1's if negative, all 0's if positive
    return (x + mask) ^ mask;
}

static inline int64 fast_abs64(int64 x) {
    int64 mask = x >> 63;  // All 1's if negative, all 0's if positive
    return (x + mask) ^ mask;
}

static void randomize_range(uint32 arr[], int m, int n) {

    int size = n - m;

    // Fill array with numbers from m to n
    for (int i = 0; i < size; i++) {
        arr[i] = m + i;
    }

    // Fisher-Yates shuffle
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        // Swap arr[i] and arr[j]
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    for (int i = 0; i < size; i++)
        print("arr[%u]:%u\n", i, arr[i]);

}

#endif //MEMORY_SMATH_H