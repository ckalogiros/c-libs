/*
 * Created by ckalo on 3/9/26.
 */
#ifndef STRING_FORMAT_H
#define STRING_FORMAT_H
#include "my_types.h"
#include "ascii_characters.h"
#include <math.h>
#include <stdarg.h>

/// cache bases in a table, so that: int_base[4] = 10000 etc
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


/* up to 32bit integer to string conversion */
static uint32
str_conv_uint32_to_str(char* buf, const int32 number)
{
    uint32 cnt = 0;
    unsigned int num;

    if(number == 0) {
        buf[cnt++] = ASCII_0;
        return cnt;
    } else {
        num = number;
    }

    // Generate digits from end to start
    char temp[16]; // Enough for 32-bit int
    int i = 0;
    do {
        temp[i++] = ASCII_0 + (num % 10);
        num /= 10;
    } while(num > 0);

    // Copy in reverse order
    while(i > 0)
        buf[cnt++] = temp[--i];

    return cnt;
}
static uint32
str_conv_int32_to_str(char* buf, const int32 number)
{
    int32 num = number;
    uint32 cnt = 0;
    if(number < 0) {
        buf[cnt++] = '-';
        num = -num;;
    }

    cnt += str_conv_uint32_to_str(&buf[cnt], num);

    return cnt;
}

struct int_to_fract{
    int fractional_int;  // The fractional part as integer (918)
    int num_digits;      // Number of digits (4)
    int leading_num_zeroes;
};
static struct int_to_fract
fraction_to_integer(double fract_part, uint32 max_precision)
{
    int fract = 0;
    int digit_count = 0;
    int leading_zeros = 0;
    // Keep multiplying by 10 until we get all digits or hit max precision
    while (digit_count < max_precision && fract_part > 0) {
        fract_part *= 10;
        int digit = (int)fract_part;

        if(digit == 0)
            leading_zeros++;

        fract = fract * 10 + digit;
        fract_part -= digit;
        digit_count++;

        // Stop if we've extracted all significant digits, when temp is very close to 0
        if (fract_part < 0.000001) break;
    }

    struct int_to_fract int_to_fract = {
        .num_digits = digit_count,
        .fractional_int = fract,
        .leading_num_zeroes = leading_zeros,
    };

    return int_to_fract;

}

struct float_number {
    int32 int_part;
    int32 fract_part;
};
static struct float_number
str_conv_double_to_int_parts(const double number)
{
    struct float_number res;
    double num = number;
    res.int_part = (int)num;

    if(num < 0) num *= -1;

    float fract_part = num - floor(num);  // For positive numbers
    struct int_to_fract int_to_fract = fraction_to_integer(fract_part, 6);

    res.fract_part = int_to_fract.fractional_int;

    return res;
}
/**/
static void
str_conv_double_to_str(char* buf, struct float_number num)
{
    /// TODO: IMPLEMENT
    // Convert integer part
    char temp[32];
    uint32 len = str_conv_int32_to_str(temp, num.int_part);


    // Add decimal point
    *buf++ = '.';

    // // Convert fractional part
    // for (int j = 0; j < precision; j++) {
    //     frac_part *= 10;
    //     int digit = (int)frac_part;
    //     *buf++ = '0' + digit;
    //     frac_part -= digit;
    // }

    *buf = '\0';
}

/*FROM deepseek*/
// static inline void
// str_conv_double_to_str(char* buf, const double num)
// {
//
//         int int_part = (int)num;
//         // float frac_part = fabs(num - int_part);
//         float frac_part = num - int_part;
//
//         // Handle sign
//         if (num < 0) {
//             *buf++ = '-';
//             int_part = -int_part;
//         }
//
//         // Convert integer part
//         char temp[32];
//         int i = 0;
//         if (int_part == 0) {
//             temp[i++] = '0';
//         } else {
//             while (int_part > 0) {
//                 temp[i++] = '0' + (int_part % 10);
//                 int_part /= 10;
//             }
//         }
//         // Reverse temp into buf
//         while (i > 0) {
//             *buf++ = temp[--i];
//         }
//
//         // Add decimal point
//         *buf++ = '.';
//
//         // Convert fractional part
//         // for (int j = 0; j < precision; j++) {
//         //     frac_part *= 10;
//         //     int digit = (int)frac_part;
//         //     *buf++ = '0' + digit;
//         //     frac_part -= digit;
//         // }
//
//         *buf = '\0';
// }


uint32 string_format(char** const buf, const uint32 size, const char* format, ...);
uint32 string_format_va_list_call(char** const buf, const uint32 size, const char* format, va_list args);

uint32 string_buffer_push(char** const buf, const uint32 size, const char* str);

void string_format_test();

uint8 calculate_num_digits(uint64 number);

#endif //STRING_FORMAT_H