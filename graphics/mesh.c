/*
 * Created by ckalo on 6/29/26.
 */

#include "mesh.h"
#include <math.h>
#include "my_macros.h"
#include "logger.h"
#include "smath.h"


/**************************************************************************************************
 * VERTEX INDICES
 */
vind_t triangle_indices[] = {
    0, 1, 2
};
vind_t rectangle_indices[] = {
    0, 1, 2, // [0]
    0, 2, 3  // [1]
};
vind_t pentagon_indices[] = {
    0, 1, 2, // [0]
    0, 2, 3, // [1]
    0, 3, 4  // [2]
};
vind_t hexagon_indices[] = {
    0, 1, 2, // [0]
    0, 2, 3, // [1]
    0, 3, 4, // [2]
    0, 4, 5, // [3]
};
vind_t octagon_indices[] = {
    0, 1, 2, // [0]
    0, 2, 3, // [1]
    0, 3, 4, // [2]
    0, 4, 5, // [3]
    0, 5, 6, // [4]
    0, 6, 7  // [5]
};
vind_t cube_indices[] = {
    0, 1, 2, // [0]  FRONT
    0, 2, 3, // [1]
    4, 0, 3, // [2]  TOP
    4, 3, 7, // [3]
    4, 5, 1, // [4]  LEFT
    4, 1, 0, // [5]
    3, 2, 6, // [6]  RIGHT
    3, 6, 7, // [7]
    5, 4, 7, // [8]  BACK
    5, 6, 7, // [9]
    1, 5, 6, // [10] BOTTOM
    1, 6, 2  // [11]
};
vind_t octahedron_indices[] = {
    0, 1, 2,// [0]
    0, 3, 2,// [1]
    0, 3, 4,// [2]
    0, 4, 1,// [3]
    5, 1, 2,// [4]
    5, 2, 3,// [5]
    5, 3, 4,// [6]
    5, 4, 1 // [7]
};
vind_t dodecahedron_indices[] = {
     3,  11,   7, // [0]  [0]
     3,   7,  15, // [1]  [0]
     3,  15,  13, // [2]  [0]
     7,  19,  17, // [3]  [1]
     7,  17,   6, // [4]  [1]
     7,   6,  15, // [5]  [1]
    17,   4,   8, // [6]  [2]
    17,   8,  10, // [7]  [2]
    17,  10,   6, // [8]  [2]
     8,   0,  16, // [9]  [3]
     8,  16,   2, // [10] [3]
     8,   2,  10, // [11] [3]
     0,  12,   1, // [12] [4]
     0,   1,  18, // [13] [4]
     0,  18,  16, // [14] [4]
     6,  10,   2, // [15] [5]
     6,   2,  13, // [16] [5]
     6,  13,  15, // [17] [5]
     2,  16,  18, // [18] [6]
     2,  18,   3, // [19] [6]
     2,   3,  13, // [20] [6]
    18,   1,   9, // [21] [7]
    18,   9,  11, // [22] [7]
    18,  11,   3, // [23] [7]
     4,  14,  12, // [24] [8]
     4,  12,   0, // [25] [8]
     4,   0,   8, // [26] [8]
    11,   9,   5, // [27] [9]
    11,   5,  19, // [28] [9]
    11,  19,   7, // [29] [9]
    19,   5,  14, // [30] [10]
    19,  14,   4, // [31] [10]
    19,   4,  17, // [32] [10]
     1,  12,  14, // [33] [11]
     1,  14,   5, // [34] [11]
     1,   5,   9  // [35] [11]
};

#define a (0.f)
#define b (1.f)
float triangle_vertices[] = {
    -b,  b, // [0]
    -b, -b, // [1]
     b, -b  // [2]
};

float rectangle_vertices[] = {
    -b,  b, // [0]
    -b, -b, // [1]
     b, -b, // [2]
     b,  b  // [3]
};

#define s (SQRT_10_PLUS_2SQRT5 / 4.f)
#define h ((SQRT5-1.f) / 4.f)
#define d (SQRT_10_MINUS_2SQRT5 / 4.f)
#define e ((SQRT5+1.f) / 4.f)
float pentagon_vertices[] = {
    a,  b, // [0]
   -s,  h, // [1]
   -d, -e, // [2]
    d, -e, // [3]
    s,  h  // [4]
};
#undef s
#undef h
#undef d
#undef e

#define s (SQRT3/2.f) ///   0.866025403785
#define h (0.5f)
float hexagon_vertices[] = {
    a,  b, // [0]
   -s,  h, // [1]
   -s, -h, // [2]
    a, -b, // [3]
    s, -h, // [4]
    s,  h, // [5]
};

#define s (SQRT2/2.f) /// 0.70710678
float octagon_vertices[] = {
    a,  b, // [0]
   -s,  s, // [1]
   -b,  a, // [2]
   -s, -s, // [3]
    a, -b, // [4]
    s, -s, // [5]
    b,  a, // [6]
    s,  s  // [7]
};
#undef s

float cube_vertices[] = {
    -b,  b,  b, // [0]
    -b, -b,  b, // [1]
     b, -b,  b, // [2]
     b,  b,  b, // [3]
    -b,  b, -b, // [4]
    -b, -b, -b, // [5]
     b, -b, -b, // [6]
     b,  b, -b  // [7]
};

#define s SQRT2
float octahedron_vertices[] = {
    a,  s,  a, // [0]
   -b,  a,  b, // [1]
    b,  a,  b, // [2]
    b,  a, -b, // [3]
   -b,  a, -b, // [4]
    a, -s,  a  // [5]
};               
#undef s

#define d PHI
#define r (1.f / d)
float dodecahedron_vertices[]= {
    -b, -b, -b, // [0]
    -b, -b,  b, // [1]
    -b,  b, -b, // [2]
    -b,  b,  b, // [3]
     b, -b, -b, // [4]
     b, -b,  b, // [5]
     b,  b, -b, // [6]
     b,  b,  b, // [7]
     a, -r, -d, // [8]
     a, -r,  d, // [9]
     a,  r, -d, // [10]
     a,  r,  d, // [11]
    -r, -d,  a, // [12]
    -r,  d,  a, // [13]
     r, -d,  a, // [14]
     r,  d,  a, // [15]
    -d,  a, -r, // [16]
     d,  a, -r, // [17]
    -d,  a,  r, // [18]
     d,  a,  r  // [19]
};
#undef d
#undef r

/// TODO: implement n-polygon
/// TODO: implement n-polyhedron
float blender_ico_shere_vertices_indexed[] = {
    0.000000f, -1.000000f,  0.000000f,
    0.723607f, -0.447220f,  0.525725f,
   -0.276388f, -0.447220f,  0.850649f,
   -0.894426f, -0.447216f,  0.000000f,
   -0.276388f, -0.447220f, -0.850649f,
    0.723607f, -0.447220f, -0.525725f,
    0.276388f,  0.447220f,  0.850649f,
   -0.723607f,  0.447220f,  0.525725f,
   -0.723607f,  0.447220f, -0.525725f,
    0.276388f,  0.447220f, -0.850649f,
    0.894426f,  0.447216f,  0.000000f,
    0.000000f,  1.000000f,  0.000000f,
   -0.162456f, -0.850654f,  0.499995f,
    0.425323f, -0.850654f,  0.309011f,
    0.262869f, -0.525738f,  0.809012f,
    0.850648f, -0.525736f,  0.000000f,
    0.425323f, -0.850654f, -0.309011f,
   -0.525730f, -0.850652f,  0.000000f,
   -0.688189f, -0.525736f,  0.499997f,
   -0.162456f, -0.850654f, -0.499995f,
   -0.688189f, -0.525736f, -0.499997f,
    0.262869f, -0.525738f, -0.809012f,
    0.951058f,  0.000000f,  0.309013f,
    0.951058f,  0.000000f, -0.309013f,
    0.000000f,  0.000000f,  1.000000f,
    0.587786f,  0.000000f,  0.809017f,
   -0.951058f,  0.000000f,  0.309013f,
   -0.587786f,  0.000000f,  0.809017f,
   -0.587786f,  0.000000f, -0.809017f,
   -0.951058f,  0.000000f, -0.309013f,
    0.587786f,  0.000000f, -0.809017f,
    0.000000f,  0.000000f, -1.000000f,
    0.688189f,  0.525736f,  0.499997f,
   -0.262869f,  0.525738f,  0.809012f,
   -0.850648f,  0.525736f,  0.000000f,
   -0.262869f,  0.525738f, -0.809012f,
    0.688189f,  0.525736f, -0.499997f,
    0.162456f,  0.850654f,  0.499995f,
    0.525730f,  0.850652f,  0.000000f,
   -0.425323f,  0.850654f,  0.309011f,
   -0.425323f,  0.850654f, -0.309011f,
    0.162456f,  0.850654f, -0.499995f
};
void create_blender_ico_sphere(float* out)
{
    uint32 vcnt = 0;
    // float* in = dodecahedron_vertices;
}


/**************************************************************************************************
 * GETTERS
 */

float* mesh_get_mesh_vertices_from_type(enum MESH_TYPE type)
{
    switch(type) {
        case MESH_TRIANGLE     : return triangle_vertices;
        case MESH_RECTANGLE    : return rectangle_vertices;
        case MESH_PENTAGON     : return pentagon_vertices;
        case MESH_HEXAGON      : return hexagon_vertices;
        case MESH_OCTAGON      : return octagon_vertices;
        case MESH_CUBE         : return cube_vertices;
        case MESH_OCTAHEDRON   : return octahedron_vertices;
        case MESH_DODECAHEDRON : return dodecahedron_vertices;
        default: assert(0 < 0);
    }
}
vind_t* mesh_get_mesh_indices_from_type(enum MESH_TYPE type)
{
    switch(type) {
        case MESH_TRIANGLE     : return triangle_indices;
        case MESH_RECTANGLE    : return rectangle_indices;
        case MESH_PENTAGON     : return pentagon_indices;
        case MESH_HEXAGON      : return hexagon_indices;
        case MESH_OCTAGON      : return octagon_indices;
        case MESH_CUBE         : return cube_indices;
        case MESH_OCTAHEDRON   : return octahedron_indices;
        case MESH_DODECAHEDRON : return dodecahedron_indices;
        default: assert(0 < 0);
    }
}
uint32 mesh_get_mesh_vertex_count_from_type(enum MESH_TYPE type)
{
    switch(type) {
        case MESH_TRIANGLE     : return ARRAY_LENGTH(triangle_vertices)    /VERTEX_POS2;
        case MESH_RECTANGLE    : return ARRAY_LENGTH(rectangle_vertices)   /VERTEX_POS2;
        case MESH_PENTAGON     : return ARRAY_LENGTH(pentagon_vertices)    /VERTEX_POS2;
        case MESH_HEXAGON      : return ARRAY_LENGTH(hexagon_vertices)    /VERTEX_POS2;
        case MESH_OCTAGON      : return ARRAY_LENGTH(octagon_vertices)     /VERTEX_POS2;
        case MESH_CUBE         : return ARRAY_LENGTH(cube_vertices)        /VERTEX_POS3;
        case MESH_OCTAHEDRON   : return ARRAY_LENGTH(octahedron_vertices)  /VERTEX_POS3;
        case MESH_DODECAHEDRON : return ARRAY_LENGTH(dodecahedron_vertices)/VERTEX_POS3;
        default: assert(0 < 0);
    }
}
vind_t mesh_get_mesh_indices_count_from_type(enum MESH_TYPE type)
{
    switch(type) {
        case MESH_TRIANGLE     : return ARRAY_LENGTH(triangle_indices);
        case MESH_RECTANGLE    : return ARRAY_LENGTH(rectangle_indices);
        case MESH_PENTAGON     : return ARRAY_LENGTH(pentagon_indices);
        case MESH_HEXAGON      : return ARRAY_LENGTH(hexagon_indices);
        case MESH_OCTAGON      : return ARRAY_LENGTH(octagon_indices);
        case MESH_CUBE         : return ARRAY_LENGTH(cube_indices);
        case MESH_OCTAHEDRON   : return ARRAY_LENGTH(octahedron_indices);
        case MESH_DODECAHEDRON : return ARRAY_LENGTH(dodecahedron_indices);
        default: assert(0 < 0);
    }
}
uint8 mesh_get_vpos_num_elems(enum MESH_TYPE type)
{
    switch(type) {
        case MESH_TRIANGLE     : return 2;
        case MESH_RECTANGLE    : return 2;
        case MESH_PENTAGON     : return 2;
        case MESH_HEXAGON      : return 2;
        case MESH_OCTAGON      : return 2;
        case MESH_CUBE         : return 3;
        case MESH_OCTAHEDRON   : return 3;
        case MESH_DODECAHEDRON : return 3;
        default: assert(0 < 0);
    }
}
char* mesh_get_name_from_type(enum MESH_TYPE type)
{
    switch(type) {
        case MESH_TRIANGLE     : return "TRIANGLE    ";
        case MESH_RECTANGLE    : return "RECTANGLE   ";
        case MESH_PENTAGON     : return "PENTAGON    ";
        case MESH_HEXAGON      : return "HEXAGON     ";
        case MESH_OCTAGON      : return "OCTAGON     ";
        case MESH_CUBE         : return "CUBE        ";
        case MESH_OCTAHEDRON   : return "OCTAHEDRON  ";
        case MESH_DODECAHEDRON : return "DODECAHEDRON";
        default: assert(0 < 0);
    }
}


#undef a
#undef b


/***************************************************************************************************
 * saved vertex data, none indexed
 **************************************************************************************************/

// float rectangle_vertices[VERTICES_PER_QUAD*VERTEX_POS2] = {
//     -1,  1,
//     -1, -1,
//      1, -1,
//     -1,  1,
//      1, -1,
//      1,  1,
// };

// float pentagon_vertices[VERTICES_PER_PENTAGON*VERTEX_POS2] = {
//     0.0,        1.0,
//    -0.95105652, 0.30901699,
//    -0.58778525,-0.80901699,
//
//     0.0,        1.0,
//    -0.58778525,-0.80901699,
//     0.58778525,-0.80901699,
//
//     0.0,        1.0,
//     0.58778525,-0.80901699,
//     0.95105652, 0.30901699,
// };

// float octagon_vertices[VERTICES_PER_OCTAGON*VERTEX_POS2] = {
//     0.0,        1.0,
//    -0.70710678, 0.70710678,
//    -1.0,        0.0,
//
//     0.0,        1.0,
//    -1.0,        0.0,
//    -0.70710678,-0.70710678,
//
//     0.0,        1.0,
//    -0.70710678,-0.70710678,
//     0.0,       -1.0,
//
//     0.0,        1.0,
//     0.0,       -1.0,
//     0.70710678,-0.70710678,
//
//     0.0,        1.0,
//     0.70710678,-0.70710678,
//     1.0,        0.0,
//
//     0.0,        1.0,
//     1.0,        0.0,
//     0.70710678, 0.70710678,
// };

// float cube_vertices[] = {
//     -1, -1, 1, /// FRONT
//      1, -1, 1,
//      1,  1, 1,
//     -1, -1, 1,
//      1,  1, 1,
//     -1,  1, 1,
//      1, -1,-1, /// BACK
//     -1, -1,-1,
//     -1,  1,-1,
//      1, -1,-1,
//     -1,  1,-1,
//      1,  1,-1,
//     -1, -1,-1, /// LEFT
//     -1, -1, 1,
//     -1,  1, 1,
//     -1, -1,-1,
//     -1,  1, 1,
//     -1,  1,-1,
//      1, -1, 1, /// RIGHT
//      1, -1,-1,
//      1,  1,-1,
//      1, -1, 1,
//      1,  1,-1,
//      1,  1, 1,
//     -1,  1, 1, /// TOP
//      1,  1, 1,
//      1,  1,-1,
//     -1,  1, 1,
//      1,  1,-1,
//     -1,  1,-1,
//     -1, -1,-1, /// BOTTOM
//      1, -1,-1,
//      1, -1, 1,
//     -1, -1,-1,
//      1, -1, 1,
//     -1, -1, 1
// };

// float octahedron_vertices[VERTICES_PER_OCTAHEDRON*VERTEX_POS3] = {
//     /// octahedron's vertex to vertex distance is √(1² + 1² + 1²) = √(1 + 1 + 1) = √3
//     /// in order to have similar dimention with the cube
//     1.73,  0,  0, /// Face 1 (right-top-front)
//     0,  0,  1.73,
//     0,  1.73,  0,
//     1.73,  0,  0, /// Face 2 (right-bottom-front)
//     0,  0,  1.73,
//     0, -1.73,  0,
//    -1.73,  0,  0, /// Face 3 (left-top-front)
//     0,  0,  1.73,
//     0,  1.73,  0,
//    -1.73,  0,  0, /// Face 4 (left-bottom-front)
//     0,  0,  1.73,
//     0, -1.73,  0,
//     1.73,  0,  0, /// Face 5 (right-top-back)
//     0,  1.73,  0,
//     0,  0, -1.73,
//     1.73,  0,  0, /// Face 6 (right-bottom-back)
//     0, -1.73,  0,
//     0,  0, -1.73,
//    -1.73,  0,  0, /// Face 7 (left-top-back)
//     0,  1.73,  0,
//     0,  0, -1.73,
//    -1.73,  0,  0, /// Face 8 (left-bottom-back)
//     0, -1.73,  0,
//     0,  0, -1.73,
// };