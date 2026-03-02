/*
 * Created by ckalo on 1/8/26.
 */
#ifndef BUFFER_PERF_TEST_BUFFER_H
#define BUFFER_PERF_TEST_BUFFER_H


void perf_test_buffer_create_generic_vs_uint32(void);

void perf_test_buffer_remove_insert(void);

void perf_test_insert_abstruct_call(void);

void perf_test_buffer_find_tail(void);


void perf_test_buffer_with_children(void);

void perf_test_buffer_add_remove_with_children(void);

void perf_test_buffer_add_remove_with_children_mixed_capacities(void);


void perf_test_buffer_with_children_in_dif_slabs(void);

#endif //BUFFER_PERF_TEST_BUFFER_H
