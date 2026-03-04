/*
 * Created by ckalo on 3/1/26.
 */

#include "str_utils.h"

#include <assert.h>
#include <string.h>



/***************************************************************************************************
 * CONVERSION
 **************************************************************************************************/

char* /* up to 32bit integer to string conversion */
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


char* /* up to 64bit integer to string conversion */
str_conv_int64_to_str(char buffer[INT64_TO_STR_BUFFER_SIZE], const int64 number)
{
    char* ptr = buffer;
    unsigned int num;

    if(number < 0) {
        *ptr++ = '-';
        num = -number;
    } else
        num = number;

    // Generate digits from the end
    char temp[INT64_TO_STR_BUFFER_SIZE]; // Enough for 32-bit int
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

    memcpy(dest + dest_len, src, src_len + 1);
    return dest;
}
