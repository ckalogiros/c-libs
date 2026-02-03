/*
 * Created by ckalo on 2/1/26.
 */
#ifndef TESTS_ENTITY_H
#define TESTS_ENTITY_H
#include "my_types.h"


void
init_entity_table();

uint32
entity_get_id(const uint32 idx);

uint32
entity_create();




void
print_entities();

#endif //TESTS_ENTITY_H