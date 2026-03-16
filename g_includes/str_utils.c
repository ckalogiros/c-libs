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
