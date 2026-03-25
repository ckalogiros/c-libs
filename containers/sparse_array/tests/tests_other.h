/*
 * Created by ckalo on 1/8/26.
 */
#ifndef SPARSE_ARR_TEST_SPARSE_ARR_H
#define SPARSE_ARR_TEST_SPARSE_ARR_H

#include "my_types.h"

void test_create_buffer(const uint32 capacity);

void test_destroy_buffer();

void test_buffer_realloc();

void test_sparse_arr_ill_remove_insert();

void test_buffer_different_capacity_buffers();

void test_buffer_remove_elements(const uint32 capacity);

void test_buffer_insert_elements(const uint32 capacity);

void test_generic_type_buffer();

void test_sparse_arr_ree_elem();

void test_buffer_with_children();

/* test file print buffer values */
void test_sparse_arr_print_values();


#endif //SPARSE_ARR_TEST_SPARSE_ARR_H
