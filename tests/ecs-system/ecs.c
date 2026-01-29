/*
 * Created by ckalo on 1/27/26.
 */

#include "ecs.h"

#include "logger.h"

static uint32 id_cnt = 0;
static buffer_t* entity_table;
static buffer_t* pos_table;
static buffer_t* scale_table;
static buffer_t* col_table;

static void init_entity_table() {
  const uint32 capacity = CAPACITY;
  entity_table = buffer_create_uint32(capacity);
}
static void init_pos_table() {
  const uint32 capacity = CAPACITY;
  pos_table = buffer_create_vec3f(capacity);
}

static void init_scale_table() {
  const uint32 capacity = CAPACITY;
  scale_table = buffer_create_vec2f(capacity);
}

static void init_col_table() {
  const uint32 capacity = CAPACITY;
  col_table = buffer_create_vec4f(capacity);
}

uint32 entity_create();
void add_pos_component();
void add_scale_component();
void add_col_component();
void print_entities();


void game_loop() {

  while (TRUE) {

    // print fps
  }

}

void test_ecs() {

  // init
  buffer_init();
  init_entity_table();
  init_pos_table();
  init_scale_table();
  init_col_table();

  // // create
  // for (uint32 i = 0; i < CAPACITY; i++) {
  //   entity_create();
  // }
  // print_entities();

  const uint32 entity = entity_create();

  // add_pos_component(entity, (vec3f){ .color=GRAY, .pos={10,50}, .size={200,8} });
  add_pos_component(entity, (vec3f){ .x=11, .y=22, .z=33 });
  print_entities();

  add_scale_component(entity, (vec2f){ .x=1111, .y=2222, });
  print_entities();

  add_col_component(entity, (vec4f){ .b=1.2, .g=2.3, .b=3.4, .a=4.5, });
  print_entities();
}


uint32 entity_create() {

  const uint32 id = id_cnt++;
  const uint32 idx = buffer_push_uint32(id, entity_table);

  const uint32* data = entity_table->data;
  return data[idx];
}

void add_pos_component(const uint32 id, const vec3f pos) {

  buffer_set_for_insert(pos_table, id);
  vec3f* data = (vec3f*)pos_table->data;
  data[id] = pos;
  // float* data = (float*)pos_table->data;
  // data[id] = pos.x;
  // data[id+1] = pos.y;
  // data[id+2] = pos.z;
}
void add_scale_component(const uint32 id, const vec2f scale) {

  buffer_set_for_insert(scale_table, id);
  vec2f* data = (vec2f*)scale_table->data;
  data[id] = scale;
}
void add_col_component(const uint32 id, const vec4f col) {

  buffer_set_for_insert(col_table, id);
  vec4f* data = (vec4f*)col_table->data;
  data[id] = col;
}

void print_entities() {

  uint32* data = entity_table->data;
  for (uint32 i = 0; i <= entity_table->tail; i++) {
    print(
      "\n -- [%u] id = %u\n", i,
      data[i]
      );

    print("components:\n");

    vec3f* pos_data = (vec3f*)pos_table->data;
    print(
      "pos = x:%.2f, y:%.2f, z:%.2f\n",
      pos_data[i].x,
      pos_data[i].y,
      pos_data[i].z
      );
    vec2f* scale_data = (vec2f*)scale_table->data;
    print(
      "scale = x:%.2f, y:%.2f\n",
      scale_data[i].x,
      scale_data[i].y
      );
    vec4f* col_data = (vec4f*)col_table->data;
    print(
      "col = x:%.2f, y:%.2f, z:%.2f, w:%.2f\n",
      col_data[i].x,
      col_data[i].y,
      col_data[i].z,
      col_data[i].w
      );
  }
}