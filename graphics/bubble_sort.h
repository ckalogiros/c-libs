/*
 * Created by ckalo on 7/17/26.
 */
#ifndef BUBBLE_SORT_H
#define BUBBLE_SORT_H

#include "my_types.h"
#include "bubble_sort.h"
#include "shader_descriptor.h"

struct temp {
    float* src;
    enum SHADER_INPUT_FLAGS attrib_flags;
    enum SID_ID shader_input_id;
    uint16 divisor;
};

// Generic bubble sort with stack-allocated temporary buffer
void bubble_sort_generic(void *buffer, uint32 n, uint32 elem_size, int (*compare)(const void *, const void *));

// Alternative: Use a macro-based swap with a temporary pointer
// (This uses a pointer, but still no heap allocation)
void bubble_sort_generic_ptr(void *buffer, uint32 n, uint32 elem_size, int (*compare)(const void *, const void *));

// Even more efficient: Use a single pointer swap with type punning
void bubble_sort_generic_fast(void *buffer, uint32 n, uint32 elem_size, int (*compare)(const void *, const void *));

// Bubble sort for integer array
void bubble_sort_int(int arr[], int n);

// Bubble sort for character array (string)
void bubble_sort_char(char arr[], int n);

// Comparison functions
int compare_int(const void *a, const void *b);

int compare_char(const void *a, const void *b);

int compare_float(const void *a, const void *b);


// Comparison functions for generic sort
int compare_int(const void *a, const void *b);

int compare_char(const void *a, const void *b);

int compare_float(const void *a, const void *b);

int compare_struct_temp(const void* a, const void* b);


void print_int_array(int arr[], uint32 n);

void print_float_array(float arr[], uint32 n);

void print_char_array(char arr[], uint32 n);

void print_struct_temp(struct temp arr[], uint32 n);




#endif //BUBBLE_SORT_H


