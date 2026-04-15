/*
 * Created by ckalo on 2/1/26.
 */

#include "entity.h"

#include "sparse_arr.h"
#include "sparse_arr_struct.h"
#include "logger.h"

#define MIN_CAPACITY 4096


static uint32 id_cnt = 0;
static sparse_arr_t* entity_table;



void
init_entity_table()
{
    const uint32 capacity = MIN_CAPACITY;
    entity_table = sparse_arr_create_gentype(capacity, sizeof(uint32));
}

uint32
entity_get_id(const uint32 idx)
{
    const uint32* data = entity_table->data;
    return data[idx];
}


uint32
entity_create()
{
    const uint32 id = id_cnt++;
    const uint16 idx = sparse_arr_set_for_push(entity_table);
    uint32* const data = entity_table->data;
    data[idx] = id;
    return data[idx];
}



void
print_entities()
{
    uint32* data = entity_table->data;
    for(uint32 i = 0; i <= entity_table->tail; i++) {
        print(" e:%2u, ", data[i]);
        // if((i+5)%5==0) print("\n[%u]\n", i);
        if((i+5)%5==0) print("\n", i);
    }
}