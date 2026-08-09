/*
 * Created by ckalo on 7/17/26.
 */
#include "bubble_sort.h"
#include "logger.h"

// Generic bubble sort with stack-allocated temporary buffer
void
bubble_sort_generic(void* buffer, uint32 n, uint32 elem_size, int (*compare)(const void*, const void*))
{
    // Use a variable-length array (VLA) for temporary storage
    // This is allocated on the stack
    unsigned char temp[elem_size];

    int swapped;
    uint32 i, j;

    for(i = 0; i < n - 1; i++) {
        swapped = 0;
        for(j = 0; j < n - i - 1; j++) {
            // Get pointers to elements
            unsigned char* elem1 = (unsigned char*) buffer + j * elem_size;
            unsigned char* elem2 = (unsigned char*) buffer + (j + 1) * elem_size;

            if(compare(elem1, elem2) > 0) {
                // Swap elements using the stack buffer
                // Copy elem1 to temp
                for(uint32 k = 0; k < elem_size; k++) {
                    temp[k] = elem1[k];
                }
                // Copy elem2 to elem1
                for(uint32 k = 0; k < elem_size; k++) {
                    elem1[k] = elem2[k];
                }
                // Copy temp to elem2
                for(uint32 k = 0; k < elem_size; k++) {
                    elem2[k] = temp[k];
                }
                swapped = 1;
            }
        }
        if(swapped == 0)
            break;
    }
}

// Alternative: Use a macro-based swap with a temporary pointer
// (This uses a pointer, but still no heap allocation)
void
bubble_sort_generic_ptr(void* buffer, uint32 n, uint32 elem_size, int (*compare)(const void*, const void*))
{
    // Allocate temp on the stack using alloca (GCC/Clang) or a VLA
    // Using VLA for portability
    unsigned char temp[elem_size];

    int swapped;
    uint32 i, j;

    for(i = 0; i < n - 1; i++) {
        swapped = 0;
        for(j = 0; j < n - i - 1; j++) {
            unsigned char* elem1 = (unsigned char*) buffer + j * elem_size;
            unsigned char* elem2 = (unsigned char*) buffer + (j + 1) * elem_size;

            if(compare(elem1, elem2) > 0) {
                // Manual byte-by-byte swap
                for(uint32 k = 0; k < elem_size; k++) {
                    temp[k] = elem1[k];
                    elem1[k] = elem2[k];
                    elem2[k] = temp[k];
                }
                swapped = 1;
            }
        }
        if(swapped == 0)
            break;
    }
}

// Even more efficient: Use a single pointer swap with type punning
void
bubble_sort_generic_fast(void* buffer, uint32 n, uint32 elem_size, int (*compare)(const void*, const void*))
{
    // Use a fixed-size buffer for common types (optimization)
    // For larger types, VLA is used
    unsigned char temp[elem_size];

    int swapped;
    uint32 i, j;

    for(i = 0; i < n - 1; i++) {
        swapped = 0;
        for(j = 0; j < n - i - 1; j++) {
            unsigned char* elem1 = (unsigned char*) buffer + j * elem_size;
            unsigned char* elem2 = (unsigned char*) buffer + (j + 1) * elem_size;

            if(compare(elem1, elem2) > 0) {
                // Optimized swap using memcpy-like approach
                // but without the function call overhead
                unsigned char* p1 = elem1;
                unsigned char* p2 = elem2;
                unsigned char* t = temp;
                uint32 remaining = elem_size;

                // Swap in chunks (handles alignment automatically)
                while(remaining >= sizeof(uint32)) {
                    uint32 chunk = *(uint32*) p1;
                    *(uint32*) p1 = *(uint32*) p2;
                    *(uint32*) p2 = chunk;
                    p1 += sizeof(uint32);
                    p2 += sizeof(uint32);
                    remaining -= sizeof(uint32);
                }

                // Handle remaining bytes
                while(remaining--) {
                    *t = *p1;
                    *p1 = *p2;
                    *p2 = *t;
                    p1++;
                    p2++;
                    t++;
                }
                swapped = 1;
            }
        }
        if(swapped == 0)
            break;
    }
}

// Bubble sort for integer array
void
bubble_sort_int(int arr[], int n)
{
    int i, j, temp;
    int swapped;

    for(i = 0; i < n - 1; i++) {
        swapped = 0;
        for(j = 0; j < n - i - 1; j++) {
            if(arr[j] > arr[j + 1]) {
                // Swap elements
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = 1;
            }
        }
        // If no swapping occurred, array is already sorted
        if(swapped == 0)
            break;
    }
}

// Bubble sort for character array (string)
void
bubble_sort_char(char arr[], int n)
{
    int i, j;
    char temp;
    int swapped;

    for(i = 0; i < n - 1; i++) {
        swapped = 0;
        for(j = 0; j < n - i - 1; j++) {
            if(arr[j] > arr[j + 1]) {
                // Swap characters
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = 1;
            }
        }
        if(swapped == 0)
            break;
    }
}

// Comparison functions
int
compare_int(const void* a, const void* b)
{
    return *(int*) a - *(int*) b;
}

int
compare_char(const void* a, const void* b)
{
    return *(char*) a - *(char*) b;
}

int
compare_float(const void* a, const void* b)
{
    float diff = *(float*) a - *(float*) b;
    if(diff > 0) return 1;
    if(diff < 0) return -1;
    return 0;
}

int
compare_struct_attribs_per_attrib_descriptor(const void* a, const void* b)
{
    const struct attribs_per_attrib_descriptor* aa = (const struct attribs_per_attrib_descriptor*)a;
    const struct attribs_per_attrib_descriptor* bb = (const struct attribs_per_attrib_descriptor*)b;
    int diff = aa->divisor - bb->divisor;
    if(diff > 0) return 1;
    if(diff < 0) return -1;
    return 0;
}

// Helper functions
void
print_int_array(int arr[], uint32 n)
{
    for(uint32 i = 0; i < n; i++) {
        print("%d ", arr[i]);
    }
    print("\n");
}

void
print_float_array(float arr[], uint32 n)
{
    for(uint32 i = 0; i < n; i++) {
        print("%.2f ", arr[i]);
    }
    print("\n");
}

void
print_char_array(char arr[], uint32 n)
{
    for(uint32 i = 0; i < n; i++) {
        print("%c ", arr[i]);
    }
    print("\n");
}

void
print_struct_attribs_per_attrib_descriptor(struct attribs_per_attrib_descriptor arr[], uint32 n)
{
    print("%sdivisor sorted attributes: %s\n", YELLOW, CLEAR);
    for(int i = 0; i < n; i++) {
        // print("%u %u, %u, %u\n ",
        // const char* attrib_name = shaderinfo_id_to_flagname(arr[i].shader_input_id);
        // print("%s: %u ", attrib_name, arr[i].divisor);
        print("attr_num_elems: %u ", arr[i].attr_num_elems);
        print("attr_offset: %u ", arr[i].attr_offset);
        print("divisor: %u ", arr[i].divisor);
        print("\n");
    }
    print("\n");
}
