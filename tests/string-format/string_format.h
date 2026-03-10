/*
 * Created by ckalo on 3/9/26.
 */
#ifndef STRING_FORMAT_H
#define STRING_FORMAT_H
#include "my_types.h"
#include "ascii_characters.h"

/* up to 32bit integer to string conversion */
static inline uint32
str_conv_unt32_to_str(char* buf, const int32 number)
{
    uint32 cnt = 0;
    unsigned int num;

    if(number == 0) {
        buf[cnt++] = ASCII_0;
        return cnt;
    } else {
        num = number;
    }

    // Generate digits from the end
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
static inline uint32
str_conv_int32_to_str(char* buf, const int32 number)
{
    int32 num = number;
    uint32 cnt = 0;
    if(number < 0) {
        buf[cnt++] = '-';
        num = -num;;
    }

    cnt += str_conv_unt32_to_str(&buf[cnt], num);

    return cnt;
}


char* string_format(char* const buf, const char* format, ...);

#endif //STRING_FORMAT_H