/*
 * Created by ckalo on 2/6/26.
 */

#ifndef MEMORY_TESTS_H
#define MEMORY_TESTS_H

#include "mem_man_structs.h"

#define MEM_TESTS_MEM_ALIGNMENT 64

void test_mem_instance_initialization();

void test_mem_page_request();

void test_mem_alloc(void);

void test_alloc_release_insequence(const pageid_t num_pages);

void test_alloc_first_release_after(const pageid_t num_pages);

void test_alloc_first_release_after_backwards(const pageid_t num_pages);


void test_alloc_release_mixed_all();

void test_alloc_release_fragment_mem();

void test_logger_to_html();

#endif //MEMORY_TESTS_H
