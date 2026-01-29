/*
 * Created by ckalo on 1/6/26.
 */

#include "sys_utils.h"
#include <stdio.h>

void my_exit(const char* msg, const char* file, const uint32 line)
{
    printf("EXIT AT %s, line:%u\n %s\n", file, line, msg);
    exit(1);
}
