/*
 * Created by ckalo on 1/1/26.
 */
#ifndef MEMORY_LOGGER_H
#define MEMORY_LOGGER_H

#include "../internal/mem_structs.h"
#include "mem_man_internals.h"


void mem_log_print_init_mem();

void mem_log_print_terminate_mem();


void mem_log_print_info_allocation();
void mem_log_print_info_allocation();



void mem_log_print_mem();

void mem_log_print_all_used_mem();

void mem_log_print_used_mem_buf(const char* operation, const used_mem_t* const used_mem);

void mem_log_print_free_list(const free_list_t* const head);

void mem_log_print_list_node(const char* operation, const free_list_t* const node);


// external: logger to html
void logger_to_html_init();
void logger_to_html_destroy();

// ERROR handling
void mem_print_error(enum MEMORY_ERROR_CODE code);
#endif //MEMORY_LOGGER_H
