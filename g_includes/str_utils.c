/*
 * Created by ckalo on 3/1/26.
 */

#include "str_utils.h"
#include "ascii_characters.h"
#include "math.h"
#include <assert.h>
#include <string.h>



uint8
is_newline(const char c1, const char c2)
{
    if(c1 == '\\' && c2 == 'n')
        return c2;
    return 0;
}

uint8
is_numeric(char c)
{
    if(c >= numeric && c < numeric + numeric_len)
        return c;
    return 0;
}

uint8
is_alphabet(char c)
{
    if(c >= ASCII_A && c < ASCII_Z)
        return c;
    if(c >= ASCII_a && c < ASCII_z)
        return c;
    return 0;
}

uint8 calculate_num_digits(uint64 number)
{
    // Case the number is 0
    if (number == 0) return 1;

    uint64 numdigits = 0;

    // Find how many digits are in val
    while (number)
    {
        number /= 10;
        numdigits++;
    }

    return numdigits;
}


/***************************************************************************************************
 * CONVERSIONS
 **************************************************************************************************/
/* up to 32bit integer to string conversion */
uint32 /* convert uint32 to string characters */
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

uint32 /* convert int32 to string characters */
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


struct int_to_fract  /* convert fraction part to integer */
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

struct float_number  /* convert floating point to integer parts */
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

void  /* convert floating point to string characters */
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




bool // TODO: implement faster with uint64 values
str_cmp(const char* str1, const char* str2)
{

    assert(str1);
    assert(str2);

    /* check first char */
    if(*str1 == *str2) return FALSE;

    /* check lengths */
    const uint32 str_len1 = strlen(str1);
    const uint32 str_len2 = strlen(str2);
    if(str_len1 != str_len2) return FALSE;

    /* check last char */
    if(*(str1+str_len2-1) == *(str2+str_len2-1)) return FALSE;

    /* check char by char */
    for(uint32 i = 0; i < str_len1; i++) {
        if(str1[i] != str2[i]) return FALSE;
    }
    return TRUE;
}

bool
buf32_cmp(const uint32* buf1, const uint32* buf2, const uint32 buf_len1, const uint32 buf_len2)
{
    assert(buf1);
    assert(buf2);

    /* check first char */
    if(*buf1 != *buf2) return FALSE;

    /* check lengths */
    if(buf_len1 != buf_len2) return FALSE;

    /* check last char */
    if(*(buf1+buf_len2-1) != *(buf2+buf_len2-1)) return FALSE;

    /* check val by val */
    for(uint32 i = 0; i < buf_len1; i++) {
        if(buf1[i] != buf2[i]) return FALSE;
    }
    return TRUE;
}

/***************************************************************************************************
 * CONCATENATION
 **************************************************************************************************/
char*
str_concat_int32_to_str(char* dest, const uint16 dest_size, const int32 number)
{
    uint32 dest_len = strlen(dest);
    char* ptr = dest + dest_len;
    unsigned int num;
    int i = 0;

    if(number < 0) {
        *ptr++ = '-';
        num = -number;
        i++;
    } else num = number;

    // Generate digits from the end
    char temp[INT32_TO_STR_BUFFER_SIZE]; // Enough for 32-bit int

    do {
        temp[i++] = '0' + (num % 10);
        num /= 10;
    } while(num > 0);

    assert((dest_len+i+1) <= dest_size);

    // Copy in reverse order
    while(i > 0)
        *ptr++ = temp[--i];

    *ptr = '\0';

    return dest;
}

// Even faster version using memcpy
char*
str_concat_memcpy(char* dest, const char* src, const uint16 dest_size)
{
    uint32 dest_len = strlen(dest);
    uint32 src_len = strlen(src);

    assert(dest_size >= dest_len+src_len);

    memcpy((void*)dest + dest_len, src, src_len + 1);
    return dest;
}




