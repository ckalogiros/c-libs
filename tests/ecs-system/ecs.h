/*
 * Created by ckalo on 1/27/26.
 */

#ifndef TESTS_ECS_H
#define TESTS_ECS_H

#include "buffer.h"

#define CAPACITY 64

typedef vec3f pos3;
typedef vec2f scale2;
typedef vec4f col4;

static buffer_t* pos_table;
static buffer_t* scale_table;
static buffer_t* col_table;


void test_ecs();


#endif //TESTS_ECS_H
