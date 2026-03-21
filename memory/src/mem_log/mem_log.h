/*
 * Created by ckalo on 1/1/26.
 */

#ifndef MEMORY_LOGGER_H
#define MEMORY_LOGGER_H

#include "../mem_man_structs.h"
#include "mem_man_internals.h"


void mem_log_print_init_mem(void);

void mem_log_print_terminate_mem(void);

void mem_log_print_mem(const struct mem_block* const mem);

void mem_log_print_free_list(const memid_t memid);

void mem_print_mem_alloc(void* released_addr, const struct memhnd_t hnd);

void mem_print_mem_release(void* released_addr, const struct memhnd_t hnd);

// external: logger to html
void logger_to_html_init(void);

void logger_to_html_destroy(void);

// ERROR handling
void mem_print_error(enum MEMORY_ERROR_CODE code);
#endif //MEMORY_LOGGER_H
