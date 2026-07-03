/*
 * Created by ckalo on 7/3/26.
 */

#include "smath.h"
#include <stdlib.h>
#include "logger.h"


void randomize_range(uint32 arr[], int m, int n) {

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
