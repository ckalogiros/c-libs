/*
 * Created by ckalo on 1/6/26.
 */

#include "sys_utils.h"

#include "logger_color.h"

#include <stdio.h>

void my_exit(const char* msg, const char* file, const uint32 line)
{
    printf("%s%s%s\nEXIT AT %s\nline:%u\n", RED, msg, CLEAR, file, line);
    exit(1);
}
