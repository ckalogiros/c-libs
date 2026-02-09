/*
 * Created by ckalo on 12/29/25.
 */

#ifndef MEMORY_MEMORY_MANAGER_H
#define MEMORY_MEMORY_MANAGER_H

#include "my_types.h"
#include "mem_man_interface.h"

#define PAGE_SIZE 1024

// mms: memory manager system

void memory_manager_init();

void memory_manager_terminate();

struct memhnd_t mms_allocate_pages(const uint16 num_pages, enum MEM_CHUNK_SELECTOR);

// bool TEMP_free_list_insert(const memid_t id);


void memory_manager_memset(const memid_t id, const uint16 num_pages, const uint64 val);

// todo: Belongs to internal. Should not be called by anyone
void* const Temp_get_address_by_id(const struct memhnd_t hnd);


#endif //MEMORY_MEMORY_MANAGER_H
