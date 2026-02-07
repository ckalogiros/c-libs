/*
 * Created by ckalo on 1/8/26.
 */
#ifndef BUFFER_TEST_BUFFER_H
#define BUFFER_TEST_BUFFER_H

#include "my_types.h"

void test_create_buffer(const uint32 capacity);

void test_destroy_buffer();

void test_buffer_realloc();

void test_buffer_fill_remove_insert();

void test_buffer_different_capacity_buffers();

void test_buffer_remove_elements(const uint32 capacity);

void test_buffer_insert_elements(const uint32 capacity);

void test_generic_type_buffer();

void test_buffer_free_elem();

void test_buffer_with_children();

#endif //BUFFER_TEST_BUFFER_H
