/*
 * Created by ckalo on 3/9/26.
 */

#include <stdio.h>
#include "stringc.h"
#include "tests.h"

#include "logger.h"


const char* large_text =
        "000058f0 fb80c000 |abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac [    0] | ........ ........ ........ ........"
        "000058f0 fb80c020 |abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac [   32] | ........ ........ ........ ........"
        "000058f0 fb80c040 |abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac [   64] | ........ ........ ........ ........"
        "000058f0 fb80c060 |abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac [   96] | ........ ........ ........ ........"
        "000058f0 fb80c080 |abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac [  128] | ........ ........ ........ ........"
        "000058f0 fb80c0a0 |abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac [  160] | ........ ........ ........ ........"
        "000058f0 fb80c0c0 |abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac [  192] | ........ ........ ........ ........"
        "000058f0 fb80c0e0 |abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac [  224] | ........ ........ ........ ........"
        "000058f0 fb80c100 |abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac [  256] | ........ ........ ........ ........"
        "000058f0 fb80c120 |abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac [  288] | ........ ........ ........ ........"
        "000058f0 fb80c140 |abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac [  320] | ........ ........ ........ ........"
        "000058f0 fb80c160 |abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac [  352] | ........ ........ ........ ........"
        "000058f0 fb80c180 |abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac abccbcab effdfdac [  384] | ........ ........ ........ ........";


void
test_string_create_and_extend_slabs(void)
{
    /* string set (+ realloc) */
    for(uint32 i = 0; i < 40; i++) {
        string_t* str = string_create("this is a 32 byte string");
    }
}

void
test_string_set_and_realloc(void)
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
        str = string_set_cstr(str, "Hello World There Hi Aye 1991 # TERMINATE MEMORY # \nFREEING MEMORY MANAGER: 29bd326b ... OK Process finished with exit code 07");
        str = string_set_cstr(str, large_text);
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
