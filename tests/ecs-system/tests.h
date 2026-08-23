/*
 * Created by ckalo on 2/1/26.
 */
#ifndef TESTS_TESTS_H
#define TESTS_TESTS_H
#include "my_types.h"
// #include "precision_timer.h"

#define test_ecs_print_3_loops_test()\
print("%stest:%s %sadd components in seperate loops%s\n", GREEN, CLEAR, ry, CLEAR);\
print("%stest:%s %sall components in 1 loop%s\n", GREEN, CLEAR, ry, CLEAR);\
print("%stest:%s %s1 component for all attributes in 1 loop%s\n", GREEN, CLEAR, ry, CLEAR);

static const uint32 REPEAT = 10000;


void test_create_entities(const uint32 count);

void test_ecs_groups();

uint32 test_components_seperate_add(const uint32 count);

uint32 test_components_together_add(const uint32 count);

uint32 test_components_all_attribs(const uint32 count);


struct timer;

void test_perf_ecs_insert_to_comp_helper(const uint32 count, const uint32 repeat);


void test_perf_ecs_insert_to_comp();

#endif //TESTS_TESTS_H
