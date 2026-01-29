/*
 * Created by ckalo on 12/29/25.
 */

#ifndef MEMORY_MEMORY_MANAGER_H
#define MEMORY_MEMORY_MANAGER_H

#include "my_types.h"

#define PAGE_SIZE 1024

// mms: memory manager system

void memory_manager_init(const uint32 num_pages);
void memory_manager_terminate();

pageid_t mmu_allocate_pages(pageid_t num_pages);
bool TEMP_free_list_insert(const pageid_t id);
 // todo: Belongs to internal. Should not be called by anyone
void* const Temp_get_address_by_id(const pageid_t id);

// debug-print
void mem_print_free_list();

#endif //MEMORY_MEMORY_MANAGER_H
