/*
 * Created by ckalo on 3/1/26.
 */

#include "str_utils.h"

#include <assert.h>
#include <string.h>



/***************************************************************************************************
 * CONVERSION
 **************************************************************************************************/
/* up to 32bit integer to string conversion */
char*
str_conv_int32_to_str(char buffer[INT32_TO_STR_BUFFER_SIZE], const int32 number)
{
    char* ptr = buffer;
    unsigned int num;

    if(number < 0) {
        *ptr++ = '-';
        num = -number;
    } else
        num = number;

    // Generate digits from the end
    char temp[INT32_TO_STR_BUFFER_SIZE]; // Enough for 32-bit int
    int i = 0;
    do {
        temp[i++] = '0' + (num % 10);
        num /= 10;
    } while(num > 0);

    // Copy in reverse order
    while(i > 0)
        *ptr++ = temp[--i];

    *ptr = '\0';

    return buffer;
}
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


/***************************************************************************************************
 * CONCATENATION
 **************************************************************************************************/
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
