/*
 * Created by ckalo on 2/1/26.
 */
#ifndef TESTS_COMPONENT_H
#define TESTS_COMPONENT_H

#include "buffer_struct.h"
#include "my_types.h"

typedef vec3f pos3;
typedef vec2f scale2;
typedef vec4f col4;

void init_pos_table(void);

void init_scale_table(void);

void init_col_table(void);


void add_pos_component(const uint32 id, const vec3f pos);

void add_scale_component(const uint32 id, const vec2f scale);

void add_col_component(const uint32 id, const vec4f col);

struct all_attribs {
    vec4f col;
    vec3f pos;
    vec2f scale;
    // vec3f pad; // padding
    // vec4f pad2;
};

void init_all_attribs_table();

void add_all_attribs_component(const uint32 id, const struct all_attribs col);


void print_components();


#endif //TESTS_COMPONENT_H
