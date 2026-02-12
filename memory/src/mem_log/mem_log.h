/*
 * Created by ckalo on 1/1/26.
 */
#ifndef MEMORY_LOGGER_H
#define MEMORY_LOGGER_H

#include "mem_structs.h"
#include "mem_man_internals.h"


void mem_log_print_init_mem();

void mem_log_print_terminate_mem();

void mem_log_print_mem(const struct mem* const mem);

void mem_log_print_all_used_mem();

void mem_log_print_free_list_of_chunk(const memid_t chunk_id);
void mem_log_print_free_list_table();
void mem_log_print_list_node(const char* operation, const struct free_list* const node);

void mem_print_mem_alloc(void* released_addr, const struct memhnd_t hnd);
void mem_print_mem_release(void* released_addr, const struct memhnd_t hnd);


// external: logger to html
void logger_to_html_init();
void logger_to_html_destroy();

// ERROR handling
void mem_print_error(enum MEMORY_ERROR_CODE code);
#endif //MEMORY_LOGGER_H
