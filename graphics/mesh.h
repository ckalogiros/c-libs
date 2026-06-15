/*
 * Created by ckalo on 5/18/26.
 */
#ifndef GRAPHICS_TEST_MESH_H
#define GRAPHICS_TEST_MESH_H
#include <assert.h>
#include <math.h>

#include "logger.h"
#include "my_types.h"


#define VERTEX_POS2 2
#define VERTEX_POS3 3
#define VERTICES_PER_TRI 3
#define VERTICES_PER_QUAD (VERTICES_PER_TRI*2)
#define VERTICES_PER_PENTAGON (VERTICES_PER_TRI*3)
#define VERTICES_PER_OCTAGON (VERTICES_PER_TRI*6)
#define VERTICES_PER_QUAD_INDEXED 4
#define FACES_PER_CUBE (VERTICES_PER_TRI*2)
#define FACES_PER_OCTAHEDRON 8
#define FACES_PER_DODECAHEDRON 12
#define FACES_PER_ICOSAHEDRON 20


static float quad_vertices[VERTICES_PER_QUAD * VERTEX_POS2] = {
    -1, -1,
     1, -1,
     1,  1,
    -1, -1,
     1,  1,
    -1,  1
};
static float cube_vertices[FACES_PER_CUBE * VERTICES_PER_QUAD * VERTEX_POS3] = {
    -1, -1, 1, /// FRONT
     1, -1, 1,
     1,  1, 1,
    -1, -1, 1,
     1,  1, 1,
    -1,  1, 1,
     1, -1,-1, /// BACK
    -1, -1,-1,
    -1,  1,-1,
     1, -1,-1,
    -1,  1,-1,
     1,  1,-1,
    -1, -1,-1, /// LEFT
    -1, -1, 1,
    -1,  1, 1,
    -1, -1,-1,
    -1,  1, 1,
    -1,  1,-1,
     1, -1, 1, /// RIGHT
     1, -1,-1,
     1,  1,-1,
     1, -1, 1,
     1,  1,-1,
     1,  1, 1,
    -1,  1, 1, /// TOP
     1,  1, 1,
     1,  1,-1,
    -1,  1, 1,
     1,  1,-1,
    -1,  1,-1,
    -1, -1,-1, /// BOTTOM
     1, -1,-1,
     1, -1, 1,
    -1, -1,-1,
     1, -1, 1,
    -1, -1, 1
};
static float pentagon_vertices[] = {
    0.0,        1.0,
   -0.95105652, 0.30901699,
   -0.58778525,-0.80901699,

    0.0,        1.0,
    0.58778525,-0.80901699,
   -0.58778525,-0.80901699,

    0.0,        1.0,
    0.58778525,-0.80901699,
    0.95105652, 0.30901699,
};
static float octagon_vertices[] = {
    0.0,        1.0,
   -0.70710678, 0.70710678,
   -1.0,        0.0,

    0.0,        1.0,
   -1.0,        0.0,
   -0.70710678,-0.70710678,

    0.0,        1.0,
   -0.70710678,-0.70710678,
    0.0,       -1.0,

    0.0,        1.0,
    0.0,       -1.0,
    0.70710678,-0.70710678,

    0.0,        1.0,
    0.70710678,-0.70710678,
    1.0,        0.0,

    0.0,        1.0,
    1.0,        0.0,
    0.70710678, 0.70710678,


};
static float octahedron_vertices[FACES_PER_OCTAHEDRON * VERTICES_PER_TRI * VERTEX_POS3] = {
    1,  0,  0, /// Face 1 (right-top-front)
    0,  0,  1,
    0,  1,  0,
    1,  0,  0, /// Face 2 (right-bottom-front)
    0,  0,  1,
    0, -1,  0,
   -1,  0,  0, /// Face 3 (left-top-front)
    0,  0,  1,
    0,  1,  0,
   -1,  0,  0, /// Face 4 (left-bottom-front)
    0,  0,  1,
    0, -1,  0,
    1,  0,  0, /// Face 5 (right-top-back)
    0,  1,  0,
    0,  0, -1,
    1,  0,  0, /// Face 6 (right-bottom-back)
    0, -1,  0,
    0,  0, -1,
   -1,  0,  0, /// Face 7 (left-top-back)
    0,  1,  0,
    0,  0, -1,
   -1,  0,  0, /// Face 8 (left-bottom-back)
    0, -1,  0,
    0,  0, -1,
};


/* THREE.js */
static float dodecahedron_vertices[FACES_PER_DODECAHEDRON * VERTICES_PER_PENTAGON * VERTEX_POS3];
static void mesh_build_dodecahedron()
{

    uint32 i = 0;
    float _0 = .3f;
    float vv = 1.f;
    float tt = (vv + sqrt(5)) / 2;
    float rr = vv/tt;
    float in[20][VERTEX_POS3] = {
        {-vv,  -vv,  -vv },  // [0]
        {-vv,  -vv,   vv },  // [1]
        {-vv,   vv,  -vv },  // [2]
        {-vv,   vv,   vv },  // [3]
        { vv,  -vv,  -vv },  // [4]
        { vv,  -vv,   vv },  // [5]
        { vv,   vv,  -vv },  // [6]
        { vv,   vv,   vv },  // [7]
        { _0,   -rr, -tt },  // [8]
        { _0,   -rr,  tt },  // [9]
        { _0,    rr, -tt },  // [10]
        { _0,    rr,  tt },  // [11]
        {-rr,  -tt,   _0,},  // [12]
        {-rr,   tt,   _0,},  // [13]
        { rr,  -tt,   _0,},  // [14]
        { rr,   tt,   _0,},  // [15]
        {-tt,   _0,  -rr },  // [16]
        { tt,   _0,  -rr },  // [17]
        {-tt,   _0,   rr },  // [18]
        { tt,   _0,   rr }   // [19]
    };
    uint32 tri_ind[FACES_PER_DODECAHEDRON * VERTICES_PER_PENTAGON * VERTEX_POS3] = {
        3  ,  11 ,  7   , // [0]
        3  ,  7  ,  15  , // [1]
        3  ,  15 ,  13  , // [2]
        7  ,  19 ,  17  , // [3]
        7  ,  17 ,  6   , // [4]
        7  ,  6  ,  15  , // [5]
        17 ,  4  ,  8   , // [6]
        17 ,  8  ,  10  , // [7]
        17 ,  10 ,  6   , // [8]
        8  ,  0  ,  16  , // [9]
        8  ,  16 ,  2   , // [10]
        8  ,  2  ,  10  , // [11]
        0  ,  12 ,  1   , // [12]
        0  ,  1  ,  18  , // [13]
        0  ,  18 ,  16  , // [14]
        6  ,  10 ,  2   , // [15]
        6  ,  2  ,  13  , // [16]
        6  ,  13 ,  15  , // [17]
        2  ,  16 ,  18  , // [18]
        2  ,  18 ,  3   , // [19]
        2  ,  3  ,  13  , // [20]
        18 ,  1  ,  9   , // [21]
        18 ,  9  ,  11  , // [22]
        18 ,  11 ,  3   , // [23]
        4  ,  14 ,  12  , // [24]
        4  ,  12 ,  0   , // [25]
        4  ,  0  ,  8   , // [26]
        11 ,  9  ,  5   , // [27]
        11 ,  5  ,  19  , // [28]
        11 ,  19 ,  7   , // [29]
        19 ,  5  ,  14  , // [30]
        19 ,  14 ,  4   , // [31]
        19 ,  4  ,  17  , // [32]
        1  ,  12 ,  14  , // [33]
        1  ,  14 ,  5   , // [34]
        1  ,  5  ,  9   , // [35]
    };
    const uint32 total_vertex_count = FACES_PER_DODECAHEDRON * VERTICES_PER_PENTAGON * VERTEX_POS3;
    for(uint32 k = 0; k < total_vertex_count; k++) {

        dodecahedron_vertices[i++] = in[tri_ind[k]][0] * .6;
        dodecahedron_vertices[i++] = in[tri_ind[k]][1] * .6;
        dodecahedron_vertices[i++] = in[tri_ind[k]][2] * .6;
    }
}

/* BLENDER */
// // static float dodecahedron_vertices[FACES_PER_DODECAHEDRON * VERTICES_PER_PENTAGON * VERTEX_POS3] = {
// static float dodecahedron_vertices[] = {
//     /*[0] [0]*/   0.616960,   0.572590, -0.571348,
//     /*[1] [3]*/   0.616960,  -0.582110, -0.571348,
//     /*[2] [6]*/   0.616960,   0.572590,  0.583353,
//     /*[3] [9]*/   0.616960,  -0.582110,  0.583353,
//     /*[4] [12]*/  -0.537741,  0.572590, -0.571348,
//     /*[5] [15]*/  -0.537741, -0.582110, -0.571348,
//     /*[6] [18]*/  -0.537741,  0.572590,  0.583353,
//     /*[7] [21]*/  -0.537741, -0.582110,  0.583353,
//     /*[8] [24]*/   0.396432, -0.004760, -0.928170,
//     /*[9] [27]*/  -0.317212, -0.004760, -0.928170,
//     /*[10][30]*/  0.396432,  -0.004760,  0.940175,
//     /*[11][33]*/ -0.317212,  -0.004760,  0.940175,
//     /*[12][36]*/  0.973782,  0.352062,   0.006002,
//     /*[13][39]*/  0.973782, -0.361582,   0.006002,
//     /*[14][42]*/ -0.894563,  0.352062,   0.006002,
//     /*[15][45]*/ -0.894563, -0.361582,   0.006002,
//     /*[16][48]*/  0.039610,  0.929412,  -0.350820,
//     /*[17][51]*/  0.039610,  0.929412,   0.362824,
//     /*[18][54]*/  0.039610, -0.938932,  -0.350820,
//     /*[19][57]*/  0.039610, -0.938932,   0.362824,
// };
// static void create_dodecahedron(float* out)
// {
//     uint32 i = 0;
//     float* in = &dodecahedron_vertices[0];
//     {
//         //1     9    5   17
//         //9     10   5
//         //1     13   2   9
//         //13    14   2
//         //1     17   3   13
//         //17    18   3
//         //9     2    6   10
//         //2     19   6
//         //13    3    4   14
//         //3     11   4
//         //17    5    7   18
//         //5     15   7
//         //10    6    15   5
//         //6     16   15
//         //7     12   3   18
//         //12    11   3
//         //4     20   2   14
//         //20    19   2
//         //8     16   19   20
//         //16    6    19
//         //8     12   15   16
//         //12    7    15
//         //8     20   11   12
//         //20    4    11
//     }
//     {
//         // [0]   1     9    5
//         // [1]   1     5   17
//         // [2]   9     10   5
//         // [3]   1     13   2
//         // [4]   1     2    9
//         // [5]   13    14   2
//         // [6]   1     17   3
//         // [7]   1     3   13
//         // [8]   17    18   3
//         // [9]   9     2    6
//         // [10]  9     6   10
//         // [11]  2     19   6
//         // [12]  13    3    4
//         // [13]  13    4   14
//         // [14]  3     11   4
//         // [15]  17    5    7
//         // [16]  17    7   18
//         // [17]  5     15   7
//         // [18]  10    6    15
//         // [19]  10    15   5
//         // [20]  6     16   15
//         // [21]  7     12   3
//         // [22]  7     3   18
//         // [23]  12    11   3
//         // [24]  4     20   2
//         // [25]  4     2   14
//         // [26]  20    19   2
//         // [27]  8     16   19
//         // [28]  8     19   20
//         // [29]  16    6    19
//         // [30]  8     12   15
//         // [31]  8     15   16
//         // [32]  12    7    15
//         // [33]  8     20   11
//         // [34]  8     11   12
//         // [35]  20    4    11
//     }
//
//     out[i++] = in[1-1];     out[i++] = in[9-1];    out[i++] = in[5-1];
//     out[i++] = in[1-1];     out[i++] = in[5-1];    out[i++] = in[17-1];
//     out[i++] = in[9-1];     out[i++] = in[10-1];   out[i++] = in[5-1];
//     out[i++] = in[1-1];     out[i++] = in[13-1];   out[i++] = in[2-1];
//     out[i++] = in[1-1];     out[i++] = in[2-1];    out[i++] = in[9-1];
//     out[i++] = in[13-1];    out[i++] = in[14-1];   out[i++] = in[2-1];
//     out[i++] = in[1-1];     out[i++] = in[17-1];   out[i++] = in[3-1];
//     out[i++] = in[1-1];     out[i++] = in[3-1];    out[i++] = in[13-1];
//     out[i++] = in[17-1];    out[i++] = in[18-1];   out[i++] = in[3-1];
//     out[i++] = in[9-1];     out[i++] = in[2-1];    out[i++] = in[6-1];
//     out[i++] = in[9-1];     out[i++] = in[6-1];    out[i++] = in[10-1];
//     out[i++] = in[2-1];     out[i++] = in[19-1];   out[i++] = in[6-1];
//     out[i++] = in[13-1];    out[i++] = in[3-1];    out[i++] = in[4-1];
//     out[i++] = in[13-1];    out[i++] = in[4-1];    out[i++] = in[14-1];
//     out[i++] = in[3-1];     out[i++] = in[11-1];   out[i++] = in[4-1];
//     out[i++] = in[17-1];    out[i++] = in[5-1];    out[i++] = in[7-1];
//     out[i++] = in[17-1];    out[i++] = in[7-1];    out[i++] = in[18-1];
//     out[i++] = in[5-1];     out[i++] = in[15-1];   out[i++] = in[7-1];
//     out[i++] = in[10-1];    out[i++] = in[6-1];    out[i++] = in[15-1];
//     out[i++] = in[10-1];    out[i++] = in[15-1];   out[i++] = in[5-1];
//     out[i++] = in[6-1];     out[i++] = in[16-1];   out[i++] = in[15-1];
//     out[i++] = in[7-1];     out[i++] = in[12-1];   out[i++] = in[3-1];
//     out[i++] = in[7-1];     out[i++] = in[3-1];    out[i++] = in[18-1];
//     out[i++] = in[12-1];    out[i++] = in[11-1];   out[i++] = in[3-1];
//     out[i++] = in[4-1];     out[i++] = in[20-1];   out[i++] = in[2-1];
//     out[i++] = in[4-1];     out[i++] = in[2-1];    out[i++] = in[14-1];
//     out[i++] = in[20-1];    out[i++] = in[19-1];   out[i++] = in[2-1];
//     out[i++] = in[8-1];     out[i++] = in[16-1];   out[i++] = in[19-1];
//     out[i++] = in[8-1];     out[i++] = in[19-1];   out[i++] = in[20-1];
//     out[i++] = in[16-1];    out[i++] = in[6-1];    out[i++] = in[19-1];
//     out[i++] = in[8-1];     out[i++] = in[12-1];   out[i++] = in[15-1];
//     out[i++] = in[8-1];     out[i++] = in[15-1];   out[i++] = in[16-1];
//     out[i++] = in[12-1];    out[i++] = in[7-1];    out[i++] = in[15-1];
//     out[i++] = in[8-1];     out[i++] = in[20-1];   out[i++] = in[11-1];
//     out[i++] = in[8-1];     out[i++] = in[11-1];   out[i++] = in[12-1];
//     out[i++] = in[20-1];    out[i++] = in[4-1];    out[i++] = in[11-1];
//
// }


static float blender_ico_shere_vertices_indexed[] = {
    0.000000, -1.000000,  0.000000,
    0.723607, -0.447220,  0.525725,
   -0.276388, -0.447220,  0.850649,
   -0.894426, -0.447216,  0.000000,
   -0.276388, -0.447220, -0.850649,
    0.723607, -0.447220, -0.525725,
    0.276388,  0.447220,  0.850649,
   -0.723607,  0.447220,  0.525725,
   -0.723607,  0.447220, -0.525725,
    0.276388,  0.447220, -0.850649,
    0.894426,  0.447216,  0.000000,
    0.000000,  1.000000,  0.000000,
   -0.162456, -0.850654,  0.499995,
    0.425323, -0.850654,  0.309011,
    0.262869, -0.525738,  0.809012,
    0.850648, -0.525736,  0.000000,
    0.425323, -0.850654, -0.309011,
   -0.525730, -0.850652,  0.000000,
   -0.688189, -0.525736,  0.499997,
   -0.162456, -0.850654, -0.499995,
   -0.688189, -0.525736, -0.499997,
    0.262869, -0.525738, -0.809012,
    0.951058,  0.000000,  0.309013,
    0.951058,  0.000000, -0.309013,
    0.000000,  0.000000,  1.000000,
    0.587786,  0.000000,  0.809017,
   -0.951058,  0.000000,  0.309013,
   -0.587786,  0.000000,  0.809017,
   -0.587786,  0.000000, -0.809017,
   -0.951058,  0.000000, -0.309013,
    0.587786,  0.000000, -0.809017,
    0.000000,  0.000000, -1.000000,
    0.688189,  0.525736,  0.499997,
   -0.262869,  0.525738,  0.809012,
   -0.850648,  0.525736,  0.000000,
   -0.262869,  0.525738, -0.809012,
    0.688189,  0.525736, -0.499997,
    0.162456,  0.850654,  0.499995,
    0.525730,  0.850652,  0.000000,
   -0.425323,  0.850654,  0.309011,
   -0.425323,  0.850654, -0.309011,
    0.162456,  0.850654, -0.499995,
};

static void create_blender_ico_sphere(float* out)
{
    uint32 vcnt = 0;
    // float* in = dodecahedron_vertices;
}


/**************************************************************************************************/
#define PHI 1.618033988749895f
#define INV_PHI 0.618033988749895f

typedef struct { float x, y, z; } Vec3;

/// Generate 20 vertices of a regular dodecahedron (circumradius = 1)
static void generate_dodecahedron_vertices(float vertices[20]) {
    /// Normalization factor to make circumradius = 1
    float scale = 1.0f / sqrt(1.0f);

    /// Base vertices: permutations of (±1, ±1, ±1) normalized
    float cube_verts[8][3] = {
        { 1,  1,  1}, { 1,  1, -1}, { 1, -1,  1}, { 1, -1, -1},
        {-1,  1,  1}, {-1,  1, -1}, {-1, -1,  1}, {-1, -1, -1}
    };
    uint32 vcnt = 0;
    for (int i = 0; i < 8; i++) {
        float len = sqrt(cube_verts[i][0]*cube_verts[i][0] +
                         cube_verts[i][1]*cube_verts[i][1] +
                         cube_verts[i][2]*cube_verts[i][2]);
        vertices[vcnt++] = cube_verts[i][0] / len;
        vertices[vcnt++] = cube_verts[i][1] / len;
        vertices[vcnt++] = cube_verts[i][2] / len;
    }

    /// Base vertices: permutations of (0, ±1/φ, ±φ) normalized
    float golden_verts[12][3] = {
        { 0,  INV_PHI,  PHI}, { 0,  INV_PHI, -PHI},
        { 0, -INV_PHI,  PHI}, { 0, -INV_PHI, -PHI},
        { INV_PHI,  PHI, 0}, { INV_PHI, -PHI, 0},
        {-INV_PHI,  PHI, 0}, {-INV_PHI, -PHI, 0},
        { PHI, 0,  INV_PHI}, { PHI, 0, -INV_PHI},
        {-PHI, 0,  INV_PHI}, {-PHI, 0, -INV_PHI}
    };

    vcnt = 0;
    for (int i = 0; i < 12; i++) {
        float len = sqrt(golden_verts[i][0]*golden_verts[i][0] +
                         golden_verts[i][1]*golden_verts[i][1] +
                         golden_verts[i][2]*golden_verts[i][2]);
        vertices[8 + vcnt++] = golden_verts[i][0] / len;
        vertices[8 + vcnt++] = golden_verts[i][1] / len;
        vertices[8 + vcnt++] = golden_verts[i][2] / len;
    }
}

/// Face indices (12 faces, each with 5 vertex indices)
static const int face_indices[12][5] = {
    { 0,  8, 12, 16,  4},  // face 0 (top front)
    { 0,  4,  6, 10,  2},  // face 1 (front)
    { 0,  2,  3,  7,  1},  // face 2 (right front)
    { 0,  1, 17, 12,  8},  // face 3 (top right)
    { 1,  7, 11,  9, 17},  // face 4 (right back)
    { 2, 10, 13, 15,  3},  // face 5 (bottom front)
    { 3, 15, 19, 11,  7},  // face 6 (bottom back)
    { 4, 16, 18, 14,  9},  // face 7 (top left)
    { 4,  9, 11, 19, 18},  // face 8 (left back)
    { 5, 14, 18, 19, 15},  // face 9 (left bottom)
    { 5, 15, 13, 10,  6},  // face 10 (bottom)
    { 5,  6,  4, 16, 18}   // face 11 (back)
};

/// Triangulate a pentagon (fan from first vertex)
static void triangulate_pentagon(const float vertices[], const int pentagon[5],
                          float* out, int* offset) {
    /// Triangle 1: (0, 1, 2)
    out[(*offset)++] = vertices[pentagon[0]];
    out[(*offset)++] = vertices[pentagon[1]];
    out[(*offset)++] = vertices[pentagon[2]];

    /// Triangle 2: (0, 2, 3)
    out[(*offset)++] = vertices[pentagon[0]];
    out[(*offset)++] = vertices[pentagon[2]];
    out[(*offset)++] = vertices[pentagon[3]];

    /// Triangle 3: (0, 3, 4)
    out[(*offset)++] = vertices[pentagon[0]];
    out[(*offset)++] = vertices[pentagon[3]];
    out[(*offset)++] = vertices[pentagon[4]];
}

/// Generate complete dodecahedron triangles (108 vertices = 36 triangles)
/// Returns number of vertices (always 108)
static int generate_dodecahedron_triangles(float* out_triangles) {
    float vertices[20*3];
    generate_dodecahedron_vertices(vertices);

    int offset = 0;
    for (int f = 0; f < 12; f++) {
        triangulate_pentagon(vertices, face_indices[f], out_triangles, &offset);
    }
    for (int f = 0; f < 108; f++) {
        // if((f%3)==0) print("\n[%u]:", f);
        if((f%3)==0) print("\n");
        print("%f, ", out_triangles[f]);
    }
    return offset;  /// Returns 108
}
/**************************************************************************************************/

static inline float* mesh_create_tri(float* const b, const uint16 size)
{
    assert(size == VERTEX_POS3);
    uint8 n = 0;
    const float x = 1, y = 1, z = 0;

    b[n++] = -x; b[n++] = -y;
    b[n++] =  x; b[n++] = -y;
    b[n++] =  x; b[n++] =  y;

    return b;
}

static inline float* mesh_create_quad(float* const b, const uint16 size)
{
    assert(size == VERTICES_PER_QUAD * VERTEX_POS2);
    uint8 n = 0;
    const float x = 1, y = 1, z = 0;

    b[n++] = -x; b[n++] = -y;
    b[n++] =  x; b[n++] = -y;
    b[n++] =  x; b[n++] =  y;

    b[n++] = -x; b[n++] = -y;
    b[n++] =  x; b[n++] =  y;
    b[n++] = -x; b[n++] =  y;
    return b;
}

static inline float* mesh_create_quad_indexed(float* const b, const uint16 size)
{
    assert(size == VERTICES_PER_QUAD_INDEXED * VERTEX_POS3);
    uint8 n = 0;
    const float x = 1, y = 1, z = 1;

    b[n++] = -x; b[n++] = -y;
    b[n++] =  x; b[n++] = -y;
    b[n++] =  x; b[n++] =  y;

    b[n++] = -x; b[n++] =  y;
    return b;
}

static inline float* mesh_create_cube(float* const b, const uint16 size)
{
    assert(size == VERTICES_PER_QUAD * FACES_PER_CUBE * VERTEX_POS3);
    uint8 n = 0;
    const float x = 1, y = 1, z = 1;

    b[n++] = -x; b[n++] = -y; b[n++] =  z; /// FRONT
    b[n++] =  x; b[n++] = -y; b[n++] =  z;
    b[n++] =  x; b[n++] =  y; b[n++] =  z;
    b[n++] = -x; b[n++] = -y; b[n++] =  z;
    b[n++] =  x; b[n++] =  y; b[n++] =  z;
    b[n++] = -x; b[n++] =  y; b[n++] =  z;
    b[n++] =  x; b[n++] = -y; b[n++] = -z; /// BACK
    b[n++] = -x; b[n++] = -y; b[n++] = -z;
    b[n++] = -x; b[n++] =  y; b[n++] = -z;
    b[n++] =  x; b[n++] = -y; b[n++] = -z;
    b[n++] = -x; b[n++] =  y; b[n++] = -z;
    b[n++] =  x; b[n++] =  y; b[n++] = -z;
    b[n++] = -x; b[n++] = -y; b[n++] = -z; /// LEFT
    b[n++] = -x; b[n++] = -y; b[n++] =  z;
    b[n++] = -x; b[n++] =  y; b[n++] =  z;
    b[n++] = -x; b[n++] = -y; b[n++] = -z;
    b[n++] = -x; b[n++] =  y; b[n++] =  z;
    b[n++] = -x; b[n++] =  y; b[n++] = -z;
    b[n++] =  x; b[n++] = -y; b[n++] =  z; /// RIGHT
    b[n++] =  x; b[n++] = -y; b[n++] = -z;
    b[n++] =  x; b[n++] =  y; b[n++] = -z;
    b[n++] =  x; b[n++] = -y; b[n++] =  z;
    b[n++] =  x; b[n++] =  y; b[n++] = -z;
    b[n++] =  x; b[n++] =  y; b[n++] =  z;
    b[n++] = -x; b[n++] =  y; b[n++] =  z; /// TOP
    b[n++] =  x; b[n++] =  y; b[n++] =  z;
    b[n++] =  x; b[n++] =  y; b[n++] = -z;
    b[n++] = -x; b[n++] =  y; b[n++] =  z;
    b[n++] =  x; b[n++] =  y; b[n++] = -z;
    b[n++] = -x; b[n++] =  y; b[n++] = -z;
    b[n++] = -x; b[n++] = -y; b[n++] = -z; /// BOTTOM
    b[n++] =  x; b[n++] = -y; b[n++] = -z;
    b[n++] =  x; b[n++] = -y; b[n++] =  z;
    b[n++] = -x; b[n++] = -y; b[n++] = -z;
    b[n++] =  x; b[n++] = -y; b[n++] =  z;
    b[n++] = -x; b[n++] = -y; b[n++] =  z;
    return b;
}

/* optimized: (8 vertices) least number of vertices(no texture or normal attributes) */
static inline float* mesh_create_cube_indexed_optimized(float* const b, const uint16 size, const float x, const float y, const float z)
{ /// TODO
    assert(size == VERTICES_PER_QUAD * VERTEX_POS3);
    uint8 n = 0;
    return b;
}

/* semi_optimized: (24 vertices) least number of vertices with texture or normal attributes */
static inline float* mesh_create_cube_indexed_semi_optimized(float* const b, const uint16 size, const float x, const float y, const float z)
{ /// TODO
    assert(size == VERTICES_PER_QUAD * VERTEX_POS3);
    uint8 n = 0;
    return b;
}


#endif //GRAPHICS_TEST_MESH_H