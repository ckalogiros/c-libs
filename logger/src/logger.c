#include "logger.h"
#include <stdio.h>
#include <stdarg.h>

void print(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}


void logger_test_variadic(const uint32 args_count, ...)
{
    va_list args;
    va_start(args, args_count);
    int total = 0;
    for (int i = 0; i < args_count; i++)
    {
        // if arg is a string, then the returned value is a pointer
        const uint64 value = va_arg(args, uint64);
        total        += value;
    }
    va_end(args);
}


void logger_print_sample1()
{
    void* p = NULL;
    printf(
    "address:%s%llx%s\n"
    "address:%s%llx%s\n"
    "address:%s%llx%s\n"
    "address:%s%llx%s\n"
    "address:%s%llx%s\n"
    "address:%s%llx%s\n"
    "address:%s%llx%s\n"
    "address:%s%llx%s\n"
    "address:%s%llx%s\n"
    "address:%s%llx%s\n"
    "address:%s%llx%s\n",
    RED, (uint64)&p, CLEAR,
    ry, (uint64)&p, CLEAR,
    // ry2, &text, CLEAR,
    YELLOW, (uint64)&p, CLEAR,
    yg, (uint64)&p, CLEAR,
    // yg2, &text, CLEAR,
    GREEN, (uint64)&p, CLEAR,
    gc, (uint64)&p, CLEAR,
    CYAN, (uint64)&p, CLEAR,
    // cb, &text, CLEAR,
    BLUE, (uint64)&p, CLEAR,
    br1, (uint64)&p, CLEAR,
    br2, (uint64)&p, CLEAR,
    br3, (uint64)&p, CLEAR
);
}
