/*
 * Created by ckalo on 2/1/26.
 */

#include "component.h"

#include "sparse_arr.h"
#include "sparse_arr_struct.h"
#include "logger.h"


#define MIN_CAPACITY 4096

static sparse_arr_t* pos_table;
static sparse_arr_t* scale_table;
static sparse_arr_t* col_table;

static sparse_arr_t* all_attribs_table;



void
init_pos_table(void)
{
    pos_table = sparse_arr_create_gentype(MIN_CAPACITY, sizeof(vec3f));
}

void
init_scale_table(void)
{
    scale_table = sparse_arr_create_gentype(MIN_CAPACITY, sizeof(vec2f));
}

void
init_col_table(void)
{
    col_table = sparse_arr_create_gentype(MIN_CAPACITY, sizeof(vec4f));
}


void
add_pos_component(const uint32 id, const vec3f pos)
{
    sparse_arr_set_for_insert(pos_table, id);
    vec3f* data = (vec3f*) pos_table->data;
    data[id] = pos;
}

void
add_scale_component(const uint32 id, const vec2f scale)
{
    sparse_arr_set_for_insert(scale_table, id);
    vec2f* data = (vec2f*) scale_table->data;
    data[id] = scale;
}

void
add_col_component(const uint32 id, const vec4f col)
{
    sparse_arr_set_for_insert(col_table, id);
    vec4f* data = (vec4f*) col_table->data;
    data[id] = col;
}


/*
 * TEST- AoS of basic attributes
 */
void init_all_attribs_table()
{
    all_attribs_table = sparse_arr_create_gentype(MIN_CAPACITY, sizeof(struct all_attribs));
}

void add_all_attribs_component(const uint32 id, const struct all_attribs all)
{
    sparse_arr_set_for_insert(all_attribs_table, id);

    struct all_attribs* data = (struct all_attribs*) all_attribs_table->data;

    data[id].col = all.col;
    data[id].pos = all.pos;
    data[id].scale = all.scale;

}





void
print_components()
{
    print("components:\n");
    for(uint32 i = 0; i <= pos_table->tail; i++) {
        vec3f* data = pos_table->data;
        print(
            "[%s%u%s] pos = x:%.2f, y:%.2f, z:%.2f\n",
            YELLOW, i, CLEAR,
            data[i].x,
            data[i].y,
            data[i].z
        );
    }

    for(uint32 i = 0; i <= pos_table->tail; i++) {
        vec2f* scale_data = (vec2f*) scale_table->data;
        print(
            "[%s%u%s] scale = x:%.2f, y:%.2f\n",
            YELLOW, i, CLEAR,
            scale_data[i].x,
            scale_data[i].y
        );
    }
    for(uint32 i = 0; i <= pos_table->tail; i++) {
        vec4f* col_data = (vec4f*) col_table->data;
        print(
            "[%s%u%s] col = x:%.2f, y:%.2f, z:%.2f, w:%.2f\n",
            YELLOW, i, CLEAR,
            col_data[i].x,
            col_data[i].y,
            col_data[i].z,
            col_data[i].w
        );
    }
}
