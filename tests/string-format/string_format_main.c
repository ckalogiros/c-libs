/*
 * Created by ckalo on 3/9/26.
 */

#define NUM_PAGES 10
#define CHAR_NUM_ELEMENTS (NUM_PAGES * PAGE_SIZE)
#include "logger.h"
#include "mem_man_interface.h"
#include "string_format.h"
#include "string_internals.h"


int
main(void)
{
    memman_init();
    string_container_init();

    uint32 a = 1024;
    int32 b = -123;
    double f = 3.14;
    char* s = "HELLO";
    char buf[128] = {0};
    char* str_fmt = string_format(buf, "a:%u b:%d f:%f s:%s\n", a, b, f, s);
    print("%s", str_fmt);
    // char* out[16] = {0};
    // str_conv_int32_to_str(&out[0], a);


    // buffer_init_table();


    memman_terminate();

    return 0;
}
