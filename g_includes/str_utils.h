/*
 * Created by ckalo on 3/1/26.
 */
#ifndef G_INCLUDES_STR_UTILS_H
#define G_INCLUDES_STR_UTILS_H

#include "my_types.h"


#define INT32_TO_STR_BUFFER_SIZE 16
#define INT64_TO_STR_BUFFER_SIZE 24

#define alphabet_lower 97
#define alphabet_upper 65
#define alphabet_len 26
#define numeric 48
#define numeric_len 10


/// cache bases in a table, so that: int_base[2] = 100, int_base[4] = 10000, etc
static uint64 int_base[] = {
    /*[ 0]*/ 0ULL,
    /*[ 1]*/ 10ULL,
    /*[ 2]*/ 100ULL,
    /*[ 3]*/ 1000ULL,
    /*[ 4]*/ 10000ULL,
    /*[ 5]*/ 100000ULL,
    /*[ 6]*/ 1000000ULL,
    /*[ 7]*/ 10000000ULL,
    /*[ 8]*/ 100000000ULL,
    /*[ 9]*/ 1000000000ULL,
    /*[10]*/ 10000000000ULL,
    /*[11]*/ 100000000000ULL,
    /*[12]*/ 1000000000000ULL,
};


uint8 is_newline(const char c1, const char c2);

uint8 is_numeric(char c);

uint8 is_alphabet(char c);

uint8 calculate_num_digits(uint64 number);


struct int_to_fract{
    int fractional_int;  // The fractional part as integer (918)
    int num_digits;      // Number of digits (4)
    int leading_num_zeroes;
};

struct float_number {
    int32 int_part;
    int32 fract_part;
};

/* up to 32bit integer to string conversion */
uint32 str_conv_uint32_to_str(char* buf, const int32 number);

uint32 str_conv_int32_to_str(char* buf, const int32 number);

struct float_number str_conv_double_to_int_parts(const double number);

void str_conv_double_to_str(char* buf, struct float_number num);



bool str_cmp(const char* str1, const char* str2);

bool buf32_cmp(const uint32* buf1, const uint32* buf2, const uint32 buf_len1, const uint32 buf_len2);

char* str_concat_int32_to_str(char* dest, const uint16 dest_size, const int32 number);

char* str_concat_memcpy(char* dest, const char* src, const uint16 dest_size);


#endif //G_INCLUDES_STR_UTILS_H