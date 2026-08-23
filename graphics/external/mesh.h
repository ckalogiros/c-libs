/*
 * Created by ckalo on 5/18/26.
 */
#ifndef GRAPHICS_TEST_MESH_H
#define GRAPHICS_TEST_MESH_H

#include "gfx_types.h"


#define VERTEX_POS2 (2)
#define VERTEX_POS3 (3)

#define FACES_PER_CUBE          (6)
#define FACES_PER_OCTAHEDRON    (8)
#define FACES_PER_DODECAHEDRON  (12)
#define FACES_PER_ICOSAHEDRON   (20)

#define VERTICES_PER_TRI          (3)
#define VERTICES_PER_QUAD         (2*VERTICES_PER_TRI)
#define VERTICES_PER_PENTAGON     (3*VERTICES_PER_TRI)
#define VERTICES_PER_OCTAGON      (6*VERTICES_PER_TRI)
#define VERTICES_PER_QUAD_INDEXED (4)
#define VERTICES_PER_CUBE         (FACES_PER_CUBE*VERTICES_PER_QUAD)
#define VERTICES_PER_OCTAHEDRON   (FACES_PER_OCTAHEDRON*VERTICES_PER_TRI)
#define VERTICES_PER_DODECAHEDRON (FACES_PER_DODECAHEDRON*VERTICES_PER_PENTAGON)


enum MESH_TYPE {
    MESH_NULL = 0,
    MESH_TRIANGLE,
    MESH_RECTANGLE,
    MESH_PENTAGON,
    MESH_HEXAGON,
    MESH_OCTAGON,
    MESH_CUBE,
    MESH_OCTAHEDRON,
    MESH_DODECAHEDRON,
    MESH_TYPE_COUNT,
};


/**************************************************************************************************
 * GETTERS
 **************************************************************************************************/

float* mesh_get_mesh_vertices_from_type(enum MESH_TYPE type);

uint32 mesh_get_mesh_vertex_count_from_type(enum MESH_TYPE type);

vind_t* mesh_get_mesh_indices_from_type(enum MESH_TYPE type);

vind_t mesh_get_mesh_indices_count_from_type(enum MESH_TYPE type);

uint8 mesh_get_num_elems_per_vertex(enum MESH_TYPE type);

char* mesh_get_name_from_type(enum MESH_TYPE type);


#endif //GRAPHICS_TEST_MESH_H
