/*
 * Created by ckalo on 4/16/26.
 */

#include "test.h"

#include "logger.h"
#include "shader_builder.h"


struct location {
    uint8 loc;
    uint8 size;
    uint8 type;
    uint8 offset;
};

/***************************************************************************************************
 * TEST ATTRIBUTES: SAT1
 **************************************************************************************************/
struct SHADER_VARIABLES_STRUCT sat1[] = {
    {.flag = BIT_MASK_FROM_UINT(1), .elems = 3, .selfidx = 0, .type = 0, .codename = "0",},
    {.flag = BIT_MASK_FROM_UINT(2), .elems = 2, .selfidx = 1, .type = 0, .codename = "1",},
    {.flag = BIT_MASK_FROM_UINT(3), .elems = 4, .selfidx = 2, .type = 0, .codename = "2",},
    {.flag = BIT_MASK_FROM_UINT(4), .elems = 3, .selfidx = 3, .type = 0, .codename = "3",},
    {.flag = BIT_MASK_FROM_UINT(5), .elems = 2, .selfidx = 4, .type = 0, .codename = "4",},
};
uint32 sat1_count = ARRAY_LENGTH(sat1);
/*
 * |0|0|0|1|
 * |1|2|2|2|
 * |2|3|3|3|
 * |4|4| | |
 */
struct location expected_result_sat1[] = {
    {.loc = 0, .offset = 0, .size = 3},
    {.loc = 0, .offset = 3, .size = 2},
    {.loc = 1, .offset = 1, .size = 4},
    {.loc = 2, .offset = 1, .size = 3},
    {.loc = 3, .offset = 0, .size = 2},
};


/***************************************************************************************************
 * TEST ATTRIBUTES: SAT2
 **************************************************************************************************/
struct SHADER_VARIABLES_STRUCT sat2[] = {
    {.flag = BIT_MASK_FROM_UINT(1), .elems = 4, .selfidx = 0, .type = 0, .codename = "0",},
    {.flag = BIT_MASK_FROM_UINT(2), .elems = 3, .selfidx = 1, .type = 0, .codename = "1",},
    {.flag = BIT_MASK_FROM_UINT(3), .elems = 3, .selfidx = 2, .type = 0, .codename = "2",},
    {.flag = BIT_MASK_FROM_UINT(4), .elems = 4, .selfidx = 3, .type = 0, .codename = "3",},
    {.flag = BIT_MASK_FROM_UINT(5), .elems = 2, .selfidx = 4, .type = 0, .codename = "4",},
};
uint32 sat2_count = ARRAY_LENGTH(sat2);
/*
 * |0|0|0|0|
 * |1|1|1|2|
 * |2|2|3|3|
 * |3|3|4|4|
 */
struct location expected_result_sat2[] = {
    {.loc = 0, .offset = 0, .size = 4},
    {.loc = 1, .offset = 0, .size = 3},
    {.loc = 1, .offset = 3, .size = 3},
    {.loc = 2, .offset = 2, .size = 4},
    {.loc = 3, .offset = 2, .size = 2},
};

/***************************************************************************************************
 * TEST ATTRIBUTES: SAT3
 **************************************************************************************************/
struct SHADER_VARIABLES_STRUCT sat3[] = {
    {.flag = BIT_MASK_FROM_UINT(1), .elems = 5, .selfidx = 0, .type = 0, .codename = "0",},
    {.flag = BIT_MASK_FROM_UINT(2), .elems = 3, .selfidx = 1, .type = 0, .codename = "1",},
    {.flag = BIT_MASK_FROM_UINT(3), .elems = 9, .selfidx = 2, .type = 0, .codename = "2",},
    {.flag = BIT_MASK_FROM_UINT(4), .elems = 5, .selfidx = 3, .type = 0, .codename = "3",},
    {.flag = BIT_MASK_FROM_UINT(5), .elems = 5, .selfidx = 3, .type = 0, .codename = "4",},
};
uint32 sat3_count = ARRAY_LENGTH(sat3);
/*
 * |0|0|0|0|
 * |0|1|1|1|
 * |2|2|2|2|
 * |2|2|2|2|
 * |2|3|3|3|
 * |3|3|4|4|
 * |4|4|4| |
 */
struct location expected_result_sat3[] = {
    {.loc = 0, .offset = 0, .size = 5},
    {.loc = 1, .offset = 1, .size = 3},
    {.loc = 2, .offset = 0, .size = 9},
    {.loc = 4, .offset = 1, .size = 5},
    {.loc = 5, .offset = 2, .size = 5},
};
/***************************************************************************************************
 * TEST ATTRIBUTES: SAT3
 **************************************************************************************************/
struct SHADER_VARIABLES_STRUCT sat4[] = {
    {.flag = BIT_MASK_FROM_UINT(1), .elems = 5, .selfidx = 0, .type = 0, .codename = "0",},
    {.flag = BIT_MASK_FROM_UINT(2), .elems = 5, .selfidx = 1, .type = 0, .codename = "1",},
    {.flag = BIT_MASK_FROM_UINT(3), .elems = 5, .selfidx = 2, .type = 0, .codename = "2",},
    {.flag = BIT_MASK_FROM_UINT(4), .elems = 5, .selfidx = 3, .type = 0, .codename = "3",},
    {.flag = BIT_MASK_FROM_UINT(5), .elems = 5, .selfidx = 4, .type = 0, .codename = "4",},
    {.flag = BIT_MASK_FROM_UINT(6), .elems = 5, .selfidx = 5, .type = 0, .codename = "5",},
};
uint32 sat4_count = ARRAY_LENGTH(sat4);
/*
 * |0|0|0|0|
 * |0|1|1|1|
 * |1|1|2|2|
 * |2|2|2|3|
 * |3|3|3|3|
 * |4|4|4|4|
 * |4|5|5|5|
 * |5| | | |
 * |||||
 */
struct location expected_result_sat4[] = {
    {.loc = 0, .offset = 0, .size = 5},
    {.loc = 1, .offset = 1, .size = 5},
    {.loc = 2, .offset = 2, .size = 5},
    {.loc = 3, .offset = 3, .size = 5},
    {.loc = 5, .offset = 0, .size = 5},
    {.loc = 6, .offset = 1, .size = 5},
};

void
test_(const uint32 count, struct SHADER_VARIABLES_STRUCT* arr, struct location* expected);

void
test_shader_builder()
{
    test_(sat1_count, sat1, expected_result_sat1);
    test_(sat2_count, sat2, expected_result_sat2);
    test_(sat3_count, sat3, expected_result_sat3);
    test_(sat4_count, sat4, expected_result_sat4);
}


void test_print_table(const char* output_name, const uint32 count, struct location* table);
void
test_(const uint32 count, struct SHADER_VARIABLES_STRUCT* arr, struct location* expected)
{
    print("--------------------------------------------------------\n");

    uint32 loc_cnt = 0; /// counts the locations element by element
    uint8 current_offset = 0;
    struct location result[16] = {0};

    /* algorithm 1 */
    for(uint32 i = 0; i < count; i++) {
        uint8 num_elems = arr[i].elems;
        uint8 type = arr[i].type;
        const char* name = arr[i].codename;

        // Record the start position for this attribute
        struct location loc = (struct location){.loc = loc_cnt, .size = num_elems, .type = type, .offset = current_offset};
        print("loc:%u elems:%u offset:%u {%s}\n", loc.loc, loc.size, loc.offset, name);
        result[i] = loc;

        // Advance by the number of elements this attribute occupies
        current_offset += num_elems;

        // Handle wrap to next row
        while(current_offset >= 4) {
            current_offset -= 4;
            loc_cnt++;
            if(current_offset >= 4) {
                struct location loc = (struct location){.loc = loc_cnt, .size = num_elems, .type = type, .offset = 0};
                print("loc:%u elems:%u offset:%u {%s}\n", loc.loc, loc.size, loc.offset, name);
                // result[i] = loc;
            }
        }
    }

    /**/
    test_print_table("RESULT", count, result);
    test_print_table("EXPECTED", count, expected);

    print("\n-- %s TEST PASS %s --\n", ry, CLEAR);
    for(uint32 i = 0; i < count; i++) {

        if(expected[i].loc == result[i].loc && expected[i].offset == result[i].offset) {

            print_test_result("shader builder test", TRUE, __FILE__, __LINE__, "test array sat1");
        } else {
            print_test_result("shader builder test", FALSE, __FILE__, __LINE__, "test array sat1");
            print("{%u} loc:%u expected loc:%u\n", i, result[i].loc, expected[i].loc);
            print(" offset:%u expected offset:%u\n", result[i].offset, expected[i].offset);
        }
    }

    print("--------------------------------------------------------\n\n");
}



void test_print_table(const char* output_name, const uint32 count, struct location* table)
{
    /**/
    uint32 cnt = 0;
    print("\n-- %s %s %s --\n", ry, output_name, CLEAR);
    for(uint32 i = 0; i < count; i++) {

        for(uint32 j = 0; j < table[i].size; j++) {
            cnt++;
            print("|%u", i);
            if(cnt >= 4) {
                print("|\n");
                cnt = 0;
            }
        }
    }
    print("|\n");
}