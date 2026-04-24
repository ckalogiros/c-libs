/*
* Created by ckalo on 4/20/26.
 */

#include "entity_test.h"
#include <stdlib.h>

struct entity* entity_table[4];
uint32 entity_table_count = 0;


struct entity* entity_create_buffer(const uint32 count)
{
    struct entity* entity = (struct entity*)malloc(sizeof(struct entity) * count);
    const uint32 idx = entity_table_count;
    entity_table[idx] = entity;
    entity_table_count++;

    return entity;
}
