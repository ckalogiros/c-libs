/*
 * Created by ckalo on 2/6/26.
 */
#ifndef MEMORY_TESTS_H
#define MEMORY_TESTS_H

void perf_mem_big_chunk_allocation();

void test_alloc_release_insequence_at_start_by_1();
void test_alloc_release_insequence_at_start_by_2();
void test_alloc_release_insequence_at_start_by_3();
void test_alloc_release_insequence_at_start_by_4();

void test_alloc_release_insequence_at_start_mix();

void test_alloc_release_unordered_at_start_by_1();
void test_alloc_release_unordered_at_start_by_2();
void test_alloc_release_unordered_at_start_by_3();
void test_alloc_release_unordered_at_start_by_4();

void test_alloc_release_mixed_all();
void test_alloc_release_fragment_mem();

void test_logger_to_html();

#endif //MEMORY_TESTS_H