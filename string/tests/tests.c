/*
 * Created by ckalo on 3/9/26.
 */

#include <stdio.h>
#include "stringc.h"
#include "tests.h"

#include "logger.h"

void
test_string_is_working(void)
{
    const char* text = ".-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-";
    string_t* str = string_create(text);
    printf("string:%s\n", str->data);

    /* string set (+ realloc) */
    {
        str = string_set_cstr(str, "Hello World There Hi Aye");
        printf("string:%s\n", str->data);
        str = string_set_cstr(str, "Hello World There Hi Aye, some more text");
        printf("string:%s\n", str->data);
        str = string_set_cstr(str, "Hello World There Hi Aye asjkjaksjkscuicijskcd");
        printf("string:%s\n", str->data);
        str = string_set_cstr(str, "Hello World There Hi Aye 199192897276764761736236721763812673");
        printf("string:%s\n", str->data);
    }

    print("len:%u\n"
           "size:%u\n",
           str->len,
           str->size
           );
}

void
test_string_append_and_realloc(void)
{
    const char* text = ".-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-";
    string_t* str = string_create(text);
    printf("string:%s\n", str->data);

    /* string append (+ realloc) */
    {
        str = string_append_cstr(str, text);
        printf("string:%s\n", str->data);
        str = string_append_cstr(str, text);
        printf("string:%s\n", str->data);
        str = string_append_cstr(str, text);
        printf("string:%s\n", str->data);
        str = string_append_cstr(str, text);
        printf("string:%s\n", str->data);
        str = string_append_cstr(str, text);
        printf("string:%s\n", str->data);
        str = string_append_cstr(str, text);
        printf("string:%s\n", str->data);
        str = string_append_cstr(str, text);
        printf("string:%s\n", str->data);
        str = string_append_cstr(str, text);
        printf("string:%s\n", str->data);
        str = string_append_cstr(str, text);
        printf("string:%s\n", str->data);
        str = string_append_cstr(str, text);
        printf("string:%s\n", str->data);
        str = string_append_cstr(str, text);
        printf("string:%s\n", str->data);
        str = string_append_cstr(str, text);
        printf("string:%s\n", str->data);
        str = string_append_cstr(str, text);
        printf("string:%s\n", str->data);
        str = string_append_cstr(str, text);
        printf("string:%s\n", str->data);
        str = string_append_cstr(str, text);
        printf("string:%s\n", str->data);

    }
    print("len:%u\n"
           "size:%u\n",
           str->len,
           str->size
           );
}
