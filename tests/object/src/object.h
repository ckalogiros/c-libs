/*
 * Created by ckalo on 8/20/26.
 */
#ifndef RENDER_TEST_OBJECT_H
#define RENDER_TEST_OBJECT_H
#include "my_types.h"

struct attribute_per_vertex {
    uint32 idx; /// the index to the group of buffers
};

struct attribute_per_instance {
    uint32 idx; /// the index to the group of buffers
};

struct object {
    uint32 id;
    struct attribute_per_vertex   attr_per_vertex;
    struct attribute_per_instance attr_per_inst;
};

#define OBJECT_GROUP_LENGTH 32
struct object_group {
    uint32 obj_id[OBJECT_GROUP_LENGTH];     /// the group's object ids
    uint32 obj_gen[OBJECT_GROUP_LENGTH];    /// the group's object generation
    /// maybe object_data[]
    /// or object_attributes_idxs[]
};

struct objects {
    struct object_group group;
};

#endif //RENDER_TEST_OBJECT_H