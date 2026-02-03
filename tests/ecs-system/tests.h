/*
 * Created by ckalo on 2/1/26.
 */
#ifndef TESTS_TESTS_H
#define TESTS_TESTS_H
#include "my_types.h"
// #include "precision_timer.h"


void
test_create_entities(const uint32 count);

void
test_ecs_groups();

void
test_components_seperate_add(const uint32 count);

void // adding to components in 1 for loop
test_components_together_add(const uint32 count);


struct timer;
void test_perf_ecs_insert_to_comp_helper(const uint32 count, const uint32 repeat, struct timer* t);



void test_components_all_attribs();

void
test_perf_ecs_insert_to_comp();

#endif //TESTS_TESTS_H
