/*
 * Created by ckalo on 3/1/26.
 */
#ifndef G_INCLUDES_STR_UTILS_H
#define G_INCLUDES_STR_UTILS_H

#include "my_types.h"


#define INT32_TO_STR_BUFFER_SIZE 16


char* str_conv_int32_to_str(char buffer[16], const int32 number);

char* str_concat_int32_to_str(char* dest, const uint16 dest_size, const int32 number);

char* str_concat_memcpy(char* dest, const char* src, const uint16 dest_size);

#endif //G_INCLUDES_STR_UTILS_H