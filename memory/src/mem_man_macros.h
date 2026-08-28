/*
 * Created by ckalo on 8/25/26.
 */
#ifndef MEMORY_MACROS_H
#define MEMORY_MACROS_H

#define MEMORY_POISON_SIZE 64
#define MEM_ALIGNMENT 64

#define CHUNK_FLAGS_FREE     BIT_MASK_FROM_UINT(0) // chunk is not in use
#define CHUNK_FLAGS_OCCUPIED BIT_MASK_FROM_UINT(1) // chunk is in use but has free pages


#define CHECK_MEM_NULL(p, error_code, msg) if(p==NULL){\
mem_print_error(error_code);\
my_exit(msg, __FILE__, __LINE__);\
}

#define CHECK_CONTIGUOUS_FREE_MEM(p1, offset, p2, error_code, msg)\
if((uint64*)((uint8*)p1+offset) < (uint64*)p2){\
mem_print_error(error_code);\
print("free list: %x\nnext flist:%x\n", REDUCE_ADDR(p1, F6), REDUCE_ADDR(p2, F6));\
my_exit(msg, __FILE__, __LINE__);\
}

/* FOR DEBUG */
#define MEMORY_DEBUG_FPRINT_USED_MEM(filename)\
fs_fprint_all_used_mem(filename);\

#define MEMORY_FPRINT_DEBUG_ENABLED
/* FOR DEBUG */

#endif //MEMORY_MACROS_H