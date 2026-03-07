/*
 * Created by ckalo on 2/6/26.
 */

#ifndef MEMORY_TESTS_H
#define MEMORY_TESTS_H

#include "mem_structs.h"
#include "my_types.h"

void test_mem_alloc(const pageid_t num_pages);

void test_mem_alloc1(void);

void test_alloc_release_insequence(const pageid_t num_pages, const bool print_list);

void test_alloc_first_release_after(const pageid_t num_pages, const bool print_list);

void test_alloc_first_release_after_backwards(const pageid_t num_pages, const bool print_list);


void test_alloc_release_mixed_all();

void test_alloc_release_fragment_mem();

void test_logger_to_html();

#endif //MEMORY_TESTS_H
