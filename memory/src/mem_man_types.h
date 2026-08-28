/*
 * Created by ckalo on 3/21/26.
 */
#ifndef MEMORY_TYPES_H
#define MEMORY_TYPES_H

#include "my_types.h"

#define PAGE_SIZE 1024


/* *********************************
 * memory table index */
typedef int16 memid_t;

/* *********************************
 * memory offset index */
typedef uint16  memoffset_t;

/* *********************************
 * memory pages index */
typedef int16 pageid_t;

/* ************************************
 * allocation's return type to caller */
struct memhnd_t {
    memid_t  mem_id;
    pageid_t page_id;
    memid_t  num_pages;
};


#endif //MEMORY_TYPES_H