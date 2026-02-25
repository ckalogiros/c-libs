/*
 * Created by ckalo on 1/17/26.
 */
#ifndef CHAR_BUF_H
#define CHAR_BUF_H

#include "my_types.h"
#include "sys_utils.h"

#define CHAR_BUF_16_CHECK(src, dst)\
if( !(src->len >= 0 && src->len <= 16) ){\
    my_exit("ERROR:\n in macro:CHAR_BUF_16_CHECK(src, dst)/n"\
    "source buffer WRONG length", __FILE__, __LINE__);\
}\
if( !(dst->len >= 0 && dst->len <= 16) ){\
my_exit("ERROR:\n in macro:CHAR_BUF_16_CHECK(src, dst)/n"\
"destination buffer WRONG length", __FILE__, __LINE__);\
}

typedef struct char_buf16_t {
    char buf[16];
    uint16 len;
} char_buf16_t;

static inline void
char_buf16_copy_from_str(const char* const src, char_buf16_t* const dst)
{
    uint16 i = 0;
    while(src[i] != '\0') {
        if(i > 16) break;
            // my_exit("ERROR: char_buf len", __FILE__, __LINE__);

        dst->buf[i] = src[i];
        dst->len++;
        i++;
    }
    dst->buf[i] = '\0';
}

static inline void
char_buf16_copy_from_str_append(const char* const src, char_buf16_t* const dst)
{
    uint16 i = 0;
    uint16 k = dst->len;
    while(src[i] != '\0') {
        if(i > 16 || dst->len > 16)
            my_exit("ERROR: char_buf len", __FILE__, __LINE__);

        dst->buf[k++] = src[i];
        dst->len++;
        i++;
    }
}

static inline void
char_buf16_copy(const char_buf16_t* const src, char_buf16_t* const dst)
{
    CHAR_BUF_16_CHECK(src, dst);

    for(uint16 i = 0; i < src->len; i++) {
        dst->buf[i] = src->buf[i];
    }
}

#endif //CHAR_BUF_H
