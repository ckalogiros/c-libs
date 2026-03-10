/*
 * Created by ckalo on 3/8/26.
 */
#ifndef STRING_STRING_STRUCT_H
#define STRING_STRING_STRUCT_H
#include "my_types.h"


#define STRING_MIN_LEN 8

typedef uint16 strlen_t;

enum STRING_ERROR_CODE {
    NO_STRING_TABLE_INIT = BIT_MASK_FROM_UINT(1),
};

typedef struct string_t {
    strlen_t len;
    strlen_t size;
    uint8 self_size;
    char data[1];
} string_t;

#endif //STRING_STRING_STRUCT_H