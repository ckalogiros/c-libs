#include <stdio.h>

#include "mem_man_interface.h"
#include "stringc.h"
#include "string_internals.h"

int
main(void)
{
    memman_init();
    string_container_init();
    const char* text = "A";
    string_t* str = string_create(text);
    printf("string:%s\n", str->data);

    str = string_append_cstr(str, "B");
    str = string_append_cstr(str, "B");
    str = string_append_cstr(str, "B");
    str = string_append_cstr(str, "B");
    str = string_append_cstr(str, "B");
    str = string_append_cstr(str, "B");
    str = string_append_cstr(str, "B");
    str = string_append_cstr(str, "B");
    str = string_append_cstr(str, "B");
    str = string_append_cstr(str, "B");

    printf("str:%s\n"
            "len:%u\n"
           "size:%u\n",
           str->data,
           str->len,
           str->size
           );


    memman_terminate();
    return 0;
}
