/*
 * Created by ckalo on 3/9/26.
 */

#include "test_performance.h"
#include <stdio.h>
#include <string.h>

#include "sparse_arr_internals.h"
#include "stringc.h"
#include "logger.h"
#include "precision_timer.h"
#include "precision_timer_logger.h"
#include "string_internals.h"

#define tests_print_string(str, realloc_iter)\
print("string:%s\nlen:%u\nsize:%u\ntotal size:%u\nself size:%u\nrealloc_iter:%u\n",\
str->data,str->len,str->size, str->len+str->self_size, str->self_size, realloc_iter);

void
perf_test_string_set(void)
{
    string_container_init(2);

    /* If measuring performance, with a high count of repeats,
     * dissable file print to avoid writing too much data to the file */
    const uint32 repeat = 1000000;

    const char* description =
            "1.\n"
            "  Create a string object with some text.\n"
            "2.\n"
            "  In a loop repeat setting the text of the string.\n";


    print("%s-- -- -- TEST: -- -- --%s\n", GREEN, CLEAR);
    print("%s perf_test_string_set%s\n\n", CYAN, CLEAR);
    print("%s description%s\n%s\n\n", ry, CLEAR, description);

    sparse_arr_init_table(2);
    timer_init_table();
    struct timer* t = timer_create_perf_timer();



    // const char* text = ".-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-";
    const char* text = "A";
    string_t* str = string_create(text);
    printf("string:%s\n", str->data);

    perf_timer_start(t);
    /* string set */
    for(uint32 i = 0; i < repeat; i++){

        str = string_set_cstr(str, "Hello World There Hi Aye");

    }
    perf_timer_stop(t);

    perf_timer_print(timer_print_config((timer_print_params){
        .t = t, .repeat = repeat, .buf_capacity = 0, .flags = PERF_T_ALL | PERF_T_USE_REPEAT
    }));
    perf_timer_reset(t);

    print("string:%s\nlen:%u\nsize:%u\n",
           str->data,str->len,str->size);
}

void
perf_test_string_append(void)
{
#define empty_string(str) { str = string_empty(str); realloc_iter++; }
// #define empty_string(str) {str->len = 0;}

    string_container_init(2);


    /* If measuring performance, with a high count of repeats,
 * dissable file print to avoid writing too much data to the file */
    const uint32 repeat = 10000000;

    const char* description =
            "1.\n"
            "  Create a string object with some text.\n"
            "2.\n"
            "  In a loop repeat appending to the string.\n";
            "NOTE\n"
            "  No reallocations, so we empty the string\n"
            "  when is near filling up";


    print("%s-- -- -- TEST: -- -- --%s\n", GREEN, CLEAR);
    print("%s perf_test_string_append%s\n\n", CYAN, CLEAR);
    print("%s description%s\n%s\n\n", ry, CLEAR, description);

    sparse_arr_init_table(2);
    timer_init_table();
    struct timer* t = timer_create_perf_timer();

    /***********************************************************************************************
     * string capacity: 16 characters
     **********************************************************************************************/
    const char* text_16_byte_capacity = "12345678";
    print("***************************************************************************\n"
      "  string capacity: 16 characters  ");
    {
        const char* msg = "init size:16 chars, appending:1 char per iter";

        const char* text = "A";
        const strlen_t str_len = strlen(text);
        string_t* str = string_create(text_16_byte_capacity);
        const strlen_t size_len = str->size-str->self_size;
        printf("string:%s\n", str->data);

        uint32 realloc_iter = 0;

        perf_timer_start(t);
        /* string append */
        for(uint32 i = 0; i < repeat; i++){

            if(str->len+str_len >= size_len) empty_string(str)
            str = string_append_cstr(str, text);

        }
        perf_timer_stop(t);

        perf_timer_print_msg(timer_print_config((timer_print_params){
            .t = t, .repeat = repeat, .buf_capacity = 0, .flags = PERF_T_ALL | PERF_T_USE_REPEAT
        }), msg);
        perf_timer_reset(t);

        tests_print_string(str, realloc_iter);
    }

    {
        const char* msg = "init size:16 chars, appending:2 chars per iter";

        const char* text = "AB";
        const strlen_t str_len = strlen(text);
        string_t* str = string_create(text_16_byte_capacity);
        const strlen_t size_len = str->size-str->self_size;
        printf("string:%s\n", str->data);

        uint32 realloc_iter = 0;

        perf_timer_start(t);
        /* string append */
        for(uint32 i = 0; i < repeat; i++){

            if(str->len+str_len >= size_len) empty_string(str)
            str = string_append_cstr(str, text);

        }
        perf_timer_stop(t);

        perf_timer_print_msg(timer_print_config((timer_print_params){
            .t = t, .repeat = repeat, .buf_capacity = 0, .flags = PERF_T_ALL | PERF_T_USE_REPEAT
        }), msg);
        perf_timer_reset(t);

        tests_print_string(str, realloc_iter);
    }

    {
        const char* msg = "init size:16 chars, appending:4 chars per iter";

        const char* text = "ABCD";
        const strlen_t str_len = strlen(text);
        string_t* str = string_create(text_16_byte_capacity);
        const strlen_t size_len = str->size-str->self_size;
        printf("string:%s\n", str->data);

        uint32 realloc_iter = 0;

        perf_timer_start(t);
        /* string append */
        for(uint32 i = 0; i < repeat; i++){

            if(str->len+str_len >= size_len) empty_string(str)
            str = string_append_cstr(str, text);

        }
        perf_timer_stop(t);

        perf_timer_print_msg(timer_print_config((timer_print_params){
            .t = t, .repeat = repeat, .buf_capacity = 0, .flags = PERF_T_ALL | PERF_T_USE_REPEAT
        }), msg);
        perf_timer_reset(t);

        tests_print_string(str, realloc_iter);
    }

    /***********************************************************************************************
     * string capacity: 32 characters
     **********************************************************************************************/
    const char* text_32_byte_capacity = "0123456789ABCDEFGHIJKL";
    print("***************************************************************************\n"
          "  string capacity: 32 characters  ");
    {
        const char* msg = "init size:32 chars, appending:1 char per iter";

        const char* text = "A";
        const strlen_t str_len = strlen(text);
        string_t* str = string_create(text_32_byte_capacity);
        const strlen_t size_len = str->size-str->self_size;
        printf("string:%s\n", str->data);

        uint32 realloc_iter = 0;

        perf_timer_start(t);
        /* string append */
        for(uint32 i = 0; i < repeat; i++){

            if(str->len+str_len >= size_len) empty_string(str)
            str = string_append_cstr(str, text);

        }
        perf_timer_stop(t);

        perf_timer_print_msg(timer_print_config((timer_print_params){
            .t = t, .repeat = repeat, .buf_capacity = 0, .flags = PERF_T_ALL | PERF_T_USE_REPEAT
        }), msg);
        perf_timer_reset(t);

        tests_print_string(str, realloc_iter);
    }

    {
        const char* msg = "init size:32 chars, appending:2 chars per iter";

        const char* text = "AB";
        const strlen_t str_len = strlen(text);
        string_t* str = string_create(text_32_byte_capacity);
        const strlen_t size_len = str->size-str->self_size;
        printf("string:%s\n", str->data);

        uint32 realloc_iter = 0;

        perf_timer_start(t);
        /* string append */
        for(uint32 i = 0; i < repeat; i++){

            if(str->len+str_len >= size_len) empty_string(str)
            str = string_append_cstr(str, text);

        }
        perf_timer_stop(t);

        perf_timer_print_msg(timer_print_config((timer_print_params){
            .t = t, .repeat = repeat, .buf_capacity = 0, .flags = PERF_T_ALL | PERF_T_USE_REPEAT
        }), msg);
        perf_timer_reset(t);

        tests_print_string(str, realloc_iter);
    }

    {
        const char* msg = "init size:32 chars, appending:4 chars per iter";

        const char* text = "ABCD";
        const strlen_t str_len = strlen(text);
        string_t* str = string_create(text_32_byte_capacity);
        const strlen_t size_len = str->size-str->self_size;
        printf("string:%s\n", str->data);

        uint32 realloc_iter = 0;

        perf_timer_start(t);
        /* string append */
        for(uint32 i = 0; i < repeat; i++){

            if(str->len+str_len >= size_len) empty_string(str)
            str = string_append_cstr(str, text);

        }
        perf_timer_stop(t);

        perf_timer_print_msg(timer_print_config((timer_print_params){
            .t = t, .repeat = repeat, .buf_capacity = 0, .flags = PERF_T_ALL | PERF_T_USE_REPEAT
        }), msg);
        perf_timer_reset(t);

        tests_print_string(str, realloc_iter);
    }

}

void
perf_test_string_realloc(void)
{
    /* If measuring performance, with a high count of repeats,
 * dissable file print to avoid writing too much data to the file */
    const uint32 repeat = 1000000;

    const char* description =
            "1.\n"
            "  Create a string object with some text.\n"
            "2.\n"
            "  In a loop repeat  the text of the string.\n";
            "NOTE\n"
            "  No reallocations, so we empty the string\n"
            "  when is near filling up";


    print("%s-- -- -- TEST: -- -- --%s\n", GREEN, CLEAR);
    print("%s perf_test_string_realloc%s\n\n", CYAN, CLEAR);
    print("%s description%s\n%s\n\n", ry, CLEAR, description);

    sparse_arr_init_table(2);
    timer_init_table();
    struct timer* t = timer_create_perf_timer();


    // const char buffer[]
    const char* text = "A";
    string_t* str = string_create("AB1234567890123");
    printf("string:%s\n", str->data);


    perf_timer_start(t);
    /* string append (+ realloc) */
    for(uint32 i = 0; i < repeat; i++){

        str = string_append_cstr(str, text);

        if(str->len >= str->size-2) str = string_empty(str);

    }
    perf_timer_stop(t);

    perf_timer_print(timer_print_config((timer_print_params){
        .t = t, .repeat = repeat, .buf_capacity = 0, .flags = PERF_T_ALL | PERF_T_USE_REPEAT
    }));
    perf_timer_reset(t);

    print("string:%s\nlen:%u\nsize:%u\n",
           str->data,str->len,str->size);
}


#undef tests_print_string